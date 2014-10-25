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

using namespace std;

GLVU glvu;

// GLSL forward rendering program handle
GLuint glslProgramHandle = 0;

GLint maxItLoc, zoomLoc, xcentLoc, ycentLoc, innercolLoc, outercolLoc1, outercolLoc2;
GLint crealLoc, cimagLoc;

float myvalue;
float maxIterations = 100.0f;
float zoom = 1.0f;
float xCenter = 0.0f;
float yCenter = 0.0f;
// GLfloat innerColor[3] = {1.0f, 1.0f, 1.0f};
// GLfloat outerColor1[3] = {0.8f, 0.8f, 0.8f};
// GLfloat outerColor2[3] = {0.0f, 0.0f, 0.0f};
// float cReal = 0.70176f;
// float cImag = -0.3842f;

GLfloat innerColor[3] = {1.0f, 0.0f, 1.0f};
GLfloat outerColor1[3] = {0.8, 0.8, 0.8f};
GLfloat outerColor2[3] = {0.0f, 0.0f, 0.0f};
float cReal = 0.285f;
float cImag = 0.0f;

void setupShaders()
{

  crealLoc = glGetUniformLocation(glslProgramHandle, "creal");
  glUniform1f(crealLoc, cReal);

  cimagLoc = glGetUniformLocation(glslProgramHandle, "cimag");
  glUniform1f(cimagLoc, cImag);

  maxItLoc = glGetUniformLocation(glslProgramHandle, "MaxIterations");
  glUniform1f(maxItLoc, maxIterations);

  zoomLoc = glGetUniformLocation(glslProgramHandle, "Zoom");
  glUniform1f(zoomLoc, zoom);

  xcentLoc = glGetUniformLocation(glslProgramHandle, "Xcenter");
  glUniform1f(xcentLoc, xCenter);

  ycentLoc = glGetUniformLocation(glslProgramHandle, "Ycenter");
  glUniform1f(ycentLoc, yCenter);

  innercolLoc = glGetUniformLocation(glslProgramHandle, "InnerColor");  
  glUniform3fv(innercolLoc, 3, innerColor);

  outercolLoc1 = glGetUniformLocation(glslProgramHandle, "OuterColor1");
  glUniform3fv(outercolLoc1, 3, outerColor1);

  outercolLoc2 = glGetUniformLocation(glslProgramHandle, "OuterColor2");  
  glUniform3fv(outercolLoc2, 3, outerColor2);

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
  glShaderSource(shaderHandle, 1, (const GLchar**) &srcloc, NULL); // read shader from file
  assert(shaderHandle != 0);   
  glCompileShader(shaderHandle); // compile shader

  // make sure it compiled fine
  GLint success;
  glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
  if (success)
    glAttachShader(programHandle, shaderHandle);  // attach to program
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
  

  // link program
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
    glUseProgram(glslProgramHandle);
    
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupShaders();

    // draw the textured quad
	  glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);        glVertex2f(-1.0f, -1.0f);
      glTexCoord2f(1.0f, 0.0f);        glVertex2f( 1.0f, -1.0f);
      glTexCoord2f(1.0f, 1.0f);        glVertex2f( 1.0f,  1.0f);
      glTexCoord2f(0.0f, 1.0f);        glVertex2f(-1.0f,  1.0f);      
    glEnd(); 
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
    case 'q':
      exit(0);
      break;

    case 'x':
      zoom +=0.1;
      break;
    case 'z':
      zoom -=0.1;
      break;

    case 'c':
      maxIterations -=10.0;
      break;

    case 'v':
      maxIterations +=10.0;
      break;            


    case 'p':
      yCenter +=1.0;
      break; 

    case '.':
      yCenter -=1.0;
      break; 

    case 'l':
      xCenter +=1.0;
      break; 

    case ';':
      xCenter -=1.0;
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
        cImag += 0.001;        
        break;
    case GLUT_KEY_RIGHT:
        cImag -= 0.001;
        break;
    case GLUT_KEY_UP:
        cReal += 0.001;        
        break;
    case GLUT_KEY_DOWN:
        cReal -= 0.001;        
        break;
    default:
        break;
  }
  cout << "Real: " << cReal << ", Imag: " << cImag << endl;
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
// open the GLVU window
//////////////////////////////////////////////////////////////////////////////
int glvuWindow()
{
  // setup GLUT window through glvu
  char title[] = "3D Viewer";
  glvu.Init(title,
            GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH,
            0, 0, 800, 800);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.5, 0.5, 0.5, 0.0);

  // create the deferred rendering program
  glslProgramHandle = createProgram("julia.vert", "julia.frag");

  

  glutDisplayFunc(&glutDisplay);
  glutIdleFunc(&glutIdle);
  glutKeyboardFunc(&glutKeyboard);
  glutSpecialFunc(&glutSpecial);
  glutMouseFunc(&glutMouseClick);
  glutMotionFunc(&glutMouseMotion);

  // pick a good viewing point
  glvuVec3f ModelMin(-10,-10,-10), ModelMax(10,10,10),
        Eye(0,0,3), LookAtCntr(0,0,0), Up(0,1,0); 

  float Yfov = 45;
  float Aspect = 1;
  float Near = 0.001f;
  float Far = 10.0f;
  glvu.SetAllCams(ModelMin, ModelMax, Eye, LookAtCntr, Up, Yfov, Aspect, Near, Far);

  glvuVec3f center(0, 0, 0);
  glvu.SetWorldCenter(center);

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
