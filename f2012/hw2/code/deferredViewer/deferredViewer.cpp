// GL interface elements are from:
//------------------------------------------------------------------------------
// GLVU : Copyright 1997 - 2002 
//        The University of North Carolina at Chapel Hill
//
// Permission to use, copy, modify, distribute and sell this software and its 
// documentation for any purpose is hereby granted without fee, provided that 
// the above copyright notice appear in all copies and that both that copyright 
// notice and this permission notice appear in supporting documentation. 
// Binaries may be compiled with this software without any royalties or 
// restrictions. 
//
// The University of North Carolina at Chapel Hill makes no representations 
// about the suitability of this software for any purpose. It is provided 
// "as is" without express or implied warranty.
//------------------------------------------------------------------------------
//
// GLSL elements are from Cubica:
//------------------------------------------------------------------------------
//    http://www.mat.ucsb.edu/~kim/cubica/
//
// Portions were written by Alan Bandurka at University of Saskatchewan
//------------------------------------------------------------------------------

#include <cmath>

#include <glvu.h>
#include <VEC3F.h>
#include <iostream>
#include <cassert>
#include <GLUT/glut.h>
#include "OBJ.h"

#define TEXTURE_WIDTH 800
#define TEXTURE_HEIGHT 800

using namespace std;

GLVU glvu;

// GLSL forward rendering program handle
GLuint glslSecondPassHandle = 0;

// GLSL forward rendering program handle
GLuint glslFirstPassHandle = 0;

// something to draw
OBJ bunny("bunny_smoothed.obj");

// deferred rendering buffers
GLuint deferredFboID = 0;
GLuint deferredRboID = 0;

// deferred shading buffers
GLuint deferredTex0ID = 0; // diffuse
GLuint deferredTex1ID = 0; // depth
GLuint deferredTex2ID = 0; // normal

///////////////////////////////////////////////////////////////////////
// draw the lights
///////////////////////////////////////////////////////////////////////
void drawLights()
{
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  GLfloat lightPosition0[] = {10.0, -10.0, 10.0, 1.0};
  GLfloat lightColor0[] = {0.75, 0.75, 0.75, 1.0};
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
  glEnable(GL_LIGHT0);
}

//////////////////////////////////////////////////////////////////////
// read a file to a string
//////////////////////////////////////////////////////////////////////
string readShaderFile(const char* filename)
{
  FILE *file;
  
  // Open shader file
  file = fopen(filename, "rb");
  if (file == NULL)
  {
    cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " : " << endl;
    cout << " Could not open file " << filename << "!" << endl;
    exit(0);
  }

  // find the end of the file
  fseek(file, 0, SEEK_END);

  // see how big the file is
  long charCount = ftell(file);

  // create a buffer big enough for it
  char *shaderSource = new char[charCount+1];

  // reset to the beginning of the file
  fseek(file, 0, SEEK_SET);

  // read in the file
  fread(shaderSource, sizeof(char), charCount, file);

  // close the file
  fclose(file);

  // add a terminating char
  shaderSource[charCount] = '\0';

  return string(shaderSource);
}

//////////////////////////////////////////////////////////////////////
// Prints framebuffer object status, and returns true if an error 
// occurred, false otherwise
//////////////////////////////////////////////////////////////////////
bool reportFboStatus(GLenum fboStatus)
{
  switch (fboStatus)
  {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      return false;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
      cout << " Framebuffer status is incomplete attachment!" << endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
      cout << " Framebuffer status is imcomplete missing attachment!" << endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
      cout << " Framebuffer status is incomplete dimensions!" << endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
      cout << " Framebuffer status is incomplete formats!" << endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
      cout << " Framebuffer status is incomplete draw buffer!" << endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
      cout << " Framebuffer status is incomplete read buffer!" << endl;
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      cout << " Framebuffer status is unsupported!" << endl;
      break;
    default:
      cout << " Framebuffer status is not complete; an error occurred: ";
      cout << (int)fboStatus << endl;
      break;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////
// draw to a buffer in preparation to do deferred shading
//////////////////////////////////////////////////////////////////////
void drawFirstPass()
{
  // with FBO
  // render directly to a texture
  // set the rendering destination to FBO
  glBindFramebuffer(GL_FRAMEBUFFER, deferredFboID);

  // activate all the deferred rendering buffers
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, deferredTex0ID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, deferredTex1ID);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, deferredTex2ID);

  // clear color buffer
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_ZERO, GL_SRC_ALPHA);

  // use the first pass program
  glUseProgram(glslFirstPassHandle);

  // draw the lights, but make sure it doesn't spin
  drawLights();

  glPushMatrix();
    // make sure the bunny spins
    static float angle = 0;
    glRotatef(angle, 0,0,1);
    angle++;

    // draw the bunny
    bunny.draw();
  glPopMatrix();

  // hand back the framebuffer to the default
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Mipmap the generated textures
  glBindTexture(GL_TEXTURE_2D, deferredTex0ID);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, deferredTex1ID);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, deferredTex2ID);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // unattach the deferred rendering program
  glUseProgram(0);
}

///////////////////////////////////////////////////////////////////////
// draw the deferred buffer to the screen
///////////////////////////////////////////////////////////////////////
void drawSecondPass()
{
  // set up the camera to just draw a quad
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // disable everything
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  // make sure we're rendering to the screen
  glUseProgram(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 800, 800);

  // clear framebuffer
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use the second pass program
  glUseProgram(glslSecondPassHandle);

  // attach textures to samplers in the second pass
  GLint depthTexture = glGetUniformLocation(glslSecondPassHandle, "depthTexture");  //depth
  GLint eyeTexture   = glGetUniformLocation(glslSecondPassHandle, "eyeTexture");    //normal
  GLint lightTexture = glGetUniformLocation(glslSecondPassHandle, "lightTexture");  //diffuse

  // attach the depth texture to GL_TEXTURE0
  glUniform1i(depthTexture, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, deferredTex0ID);
  
  // attach the eye texture to GL_TEXTURE1
  glUniform1i(eyeTexture, 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, deferredTex1ID);

  // attach the light texture to GL_TEXTURE2
  glUniform1i(lightTexture, 2);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, deferredTex2ID);

  // draw the textured quad
	glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);        glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);        glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);        glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);        glVertex2f(-1.0f,  1.0f);
  glEnd();

  // re-enable the camera settings from before
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  // re-enable everything
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // re-disable things
  glDisable(GL_BLEND);
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
}

//////////////////////////////////////////////////////////////////////
// attach a shader to a glsl handle
//////////////////////////////////////////////////////////////////////
void attachShader(const string& filename, GLuint& programHandle, GLenum shaderType)
{
  // load the vertex shader from a file into a string
  string shaderSource = readShaderFile(filename.c_str());

  // load and compile the vertex shader
  GLuint shaderHandle = glCreateShader(shaderType);	
  assert(shaderHandle != 0);
  
  // compile the vertex shader
  const char *srcloc = shaderSource.c_str();
  glShaderSource(shaderHandle, 1, (const GLchar**) &srcloc, NULL);
  assert(shaderHandle != 0);   
  glCompileShader(shaderHandle);

  // make sure it compiled fine
  GLint success;
  glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
  if (success)
    glAttachShader(programHandle, shaderHandle);
  else
  {
    cout << " Shader " << filename.c_str() << " didn't compile!" << endl;
    cout << endl << " Source: " << endl;
    cout << shaderSource << endl;

    GLchar infoLog[1024];
    GLsizei infoLogLength;
    glGetShaderInfoLog(shaderHandle, 1023, &infoLogLength, infoLog);
    cout << " Info log: " << infoLog << endl;
    exit(0);
  }
}

//////////////////////////////////////////////////////////////////////
// Load up fragment and vertex programs and attach them to a handle
//////////////////////////////////////////////////////////////////////
int createProgram(const string& vertexFilename, const string& fragmentFilename)
{
  GLuint glslHandle = glCreateProgram();
  if (!glslHandle)
  {
    cout << " Could not create a GLSL program! " << endl;
    exit(0);
  }

  attachShader(vertexFilename, glslHandle, GL_VERTEX_SHADER);
  attachShader(fragmentFilename, glslHandle, GL_FRAGMENT_SHADER);
  
  GLint success;
  GLchar infoLog[1024];
  GLsizei infoLogLength;
  glLinkProgram(glslHandle);
  glGetProgramiv(glslHandle, GL_LINK_STATUS, &success);
  if (!success)
  {
    cout << " Linking shader program failed!" << endl;
    glGetProgramInfoLog(glslHandle, 1023, &infoLogLength, infoLog);
    cout << " Info log: " << infoLog << endl;
  }

  return glslHandle;
}

///////////////////////////////////////////////////////////////////////
// GL and GLUT callbacks
///////////////////////////////////////////////////////////////////////
void glutDisplay()
{
  glvu.BeginFrame();
    drawFirstPass();
    drawSecondPass();
  glvu.EndFrame();
}

///////////////////////////////////////////////////////////////////////
// animate and display new result
///////////////////////////////////////////////////////////////////////
void glutIdle()
{
  glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutKeyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'C':
      {
        Camera* camera = glvu.GetCurrentCam();
        glvuVec3f eye;
        glvuVec3f ref;
        glvuVec3f up;
        camera->GetLookAtParams(&eye, &ref, &up);
        cout << " Eye(" << eye[0] << ", " << eye[1] << ", " << eye[2] << ") "<< endl;
        cout << " LookAtCntr(" << ref[0] << ", " << ref[1] << ", " << ref[2]  << ") " << endl;
        cout << " Up(" << up[0] << ", " << up[1] << ", " << up[2]  << ") " << endl;
      }
      break;
    case 'q':
      exit(0);
      break;
    default:
      break;
  }
  glvu.Keyboard(key,x,y);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutSpecial(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_LEFT:
      break;
    case GLUT_KEY_RIGHT:
      break;
    case GLUT_KEY_UP:
      break;
    case GLUT_KEY_DOWN:
      break;
  }
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutMouseClick(int button, int state, int x, int y)
{
  glvu.Mouse(button,state,x,y);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutMouseMotion(int x, int y)
{
  glvu.Motion(x,y);
}

//////////////////////////////////////////////////////////////////////////////
// Create a texture to render to
//////////////////////////////////////////////////////////////////////////////
void createTexture(GLuint& textureID)
{
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

//////////////////////////////////////////////////////////////////////////////
// open the GLVU window
//////////////////////////////////////////////////////////////////////////////
int glvuWindow()
{
  char title[] = "3D Viewer";
  glvu.Init(title,
            GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH,
            0, 0, 800, 800);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(1,1,1,0);

  // create the forward rendering program
  glslSecondPassHandle  = createProgram("secondPass.vert", "secondPass.frag");

  // create the deferred rendering program
  glslFirstPassHandle = createProgram("firstPass.vert", "firstPass.frag");

  glutDisplayFunc(&glutDisplay);
  glutIdleFunc(&glutIdle);
  glutKeyboardFunc(&glutKeyboard);
  glutSpecialFunc(&glutSpecial);
  glutMouseFunc(&glutMouseClick);
  glutMotionFunc(&glutMouseMotion);

  // pick a good viewing point
  glvuVec3f ModelMin(-10,-10,-10), ModelMax(10,10,10), 
        Eye(-0.0628432, -1.93023, 0.519877), 
        LookAtCntr(-0.0314216, -0.965116, 0.259939), 
        Up(-0.00127757, 0.260105, 0.96558);

  float Yfov = 45;
  float Aspect = 1;
  float Near = 0.001f;
  float Far = 10.0f;
  glvu.SetAllCams(ModelMin, ModelMax, Eye, LookAtCntr, Up, Yfov, Aspect, Near, Far);

  glvuVec3f center(0, 0, 0);
  glvu.SetWorldCenter(center);

  createTexture(deferredTex0ID);
  createTexture(deferredTex1ID);
  createTexture(deferredTex2ID);

  // create a framebuffer object, you need to delete them when program exits.
  glGenFramebuffers(1, &deferredFboID);
  glBindFramebuffer(GL_FRAMEBUFFER, deferredFboID);

  glGenRenderbuffers(1, &deferredRboID);
  glBindRenderbuffer(GL_RENDERBUFFER, deferredRboID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // attach a texture to FBO color attachement point
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, deferredTex0ID, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, deferredTex1ID, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, deferredTex2ID, 0);
  GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, buffers);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, deferredRboID);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // go into the callback loop
  glutMainLoop();

  // Control flow will never reach here
  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glvuWindow();

  return 1;
}
