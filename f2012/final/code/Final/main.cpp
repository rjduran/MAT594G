#include <cmath>
#include <glvu.h>
#include <VEC3F.h>
#include <iostream>
#include <cassert>
#include <GLUT/glut.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////


GLVU glvu;

// GLSL forward rendering program handle
GLuint glslProgramHandle = 0;

//bool fullscreen = false;
//int xScreenResFull = 0;
//int yScreenResFull = 0;


//GLint shadeLoc;
//float brightness = 0.0f;
//
//GLint swapLoc;
//bool swapShader = false;
//
//GLint fuzzLoc;

bool animate = false;
float theta = 0.0;

float fuzz = 0.1;
float width = 0.9;

VEC3F lightPos(0.1, 0.1, 0.1);
VEC3F stripeColor(0.0, 0.6, 1.0);

float blotchyness = 1.0;

float option = 0.0;
float amplitude = 0.0;
float scale = 50.0;
float frequency = 0.2;

float shape = 1.0;
float depth = 1.0;
VEC3F colorVar(0.0, 0.0, 0.0);
float nRings = 1.0;

//////////////////////////////////////////////////////////////////////
// Function Definitions
//////////////////////////////////////////////////////////////////////

void setupShaders();
string readShaderFile(const char* filename);
void attachShader(const string& filename, GLuint& programHandle, GLenum shaderType);
int createProgram(const string& vertexFilename, const string& fragmentFilename);
void glutDisplay();
void glutIdle();
void glutKeyboard(unsigned char key, int x, int y);
void glutSpecial(int key, int x, int y);
void glutMouseClick(int button, int state, int x, int y);
void glutMouseMotion(int x, int y);
int glvuWindow();
void runEverytime();





void setupShaders()
{
    
//    shadeLoc = glGetUniformLocation(glslProgramHandle, "brightness");
//    glUniform1f(shadeLoc, brightness);
//
//    swapLoc = glGetUniformLocation(glslProgramHandle, "swapColor");
//    glUniform1i(swapLoc, swapShader);
//
//    freqLoc = glGetUniformLocation(glslProgramHandle, "freq");
//    glUniform1f(freqLoc, frequency);
    
    
    // brick uniforms
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "BrickColor"), 1.0, 0.3, 0.2);
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "MortarColor"), 0.85, 0.86, 0.84);
//    glUniform2f(glGetUniformLocation(glslProgramHandle, "BrickSize"), 0.30, 0.15);
//    glUniform2f(glGetUniformLocation(glslProgramHandle, "BrickPct"), 0.90, 0.85);    
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "LightPosition"), 0.0, 0.0, 0.4);

    // stripes    
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "LightPosition"), lightPos.x, lightPos.y, lightPos.z);
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "LightColor"), 1.0, 1.0, 1.0);
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "EyePosition"), 0.0, 0.0, 3.0);
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "Specular"), 1.0, 1.0, 1.0);
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "Ambient"), 1.0, 1.0, 1.0);
//    glUniform1f(glGetUniformLocation(glslProgramHandle, "kd"), 1.0);
//    
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "StripeColor"), stripeColor.r, stripeColor.g, stripeColor.b);
//    glUniform3f(glGetUniformLocation(glslProgramHandle, "BackColor"), 0.0, 0.0, 0.0);    
//    glUniform1f(glGetUniformLocation(glslProgramHandle, "Width"), width);
//    glUniform1f(glGetUniformLocation(glslProgramHandle, "Fuzz"), fuzz);    
//    glUniform1f(glGetUniformLocation(glslProgramHandle, "Scale"), 10.0);
    
    
    glUniform1f(glGetUniformLocation(glslProgramHandle, "Blotchyness"), blotchyness);
    
    glUniform1f(glGetUniformLocation(glslProgramHandle, "Option"), option);
    
    glUniform1f(glGetUniformLocation(glslProgramHandle, "Amplitude"), amplitude);
    glUniform1f(glGetUniformLocation(glslProgramHandle, "Scale"), scale);    
    
    
    glUniform1f(glGetUniformLocation(glslProgramHandle, "Shape"), shape);
    glUniform1f(glGetUniformLocation(glslProgramHandle, "Depth"), depth);
    glUniform3f(glGetUniformLocation(glslProgramHandle, "ColorVar"), colorVar.r, colorVar.g, colorVar.b);
    glUniform1f(glGetUniformLocation(glslProgramHandle, "NRings"), nRings);    
    
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
// draw coordinate axes
///////////////////////////////////////////////////////////////////////
void drawAxes()
{
    // draw coordinate axes
    glPushMatrix();
    //glTranslatef(-0.1f, -0.1f, -0.1f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // x axis is red
    glColor4f(10.0f, 0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(10.0f, 0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    
    // y axis is green 
    glColor4f(0.0f, 10.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 10.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    
    // z axis is blue
    glColor4f(0.0f, 0.0f, 10.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor4f(0.0f, 0.0f, 10.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
    glLineWidth(1.0f);
    glPopMatrix();
}

void drawSphere() 
{
    double r = 1.0;
    double divisions = 10.0;
    double x, y, z, dTheta=180/divisions, dLon=360/divisions, degToRad=3.141592665885/180;
    
    for(double lat =0; lat <=180; lat+=dTheta)
    {
        
        glBegin( GL_QUAD_STRIP ) ;
        for(double lon =0 ; lon <=360 ; lon+=dLon)
        {  
            
            
            //Vertex 1
            x = r*cos(lon * degToRad) * sin(lat * degToRad) ;
            y = r*sin(lon * degToRad) * sin(lat * degToRad) ;
            z = r*cos(lat * degToRad) ;
            glNormal3d( x, y, z) ;
            glTexCoord2d(lon/360-0.25, lat/180);
            glVertex3d( x, y, z ) ;
            
            
            //Vetex 2
            x = r*cos(lon * degToRad) * sin( (lat + dTheta)* degToRad) ;
            y = r*sin(lon * degToRad) * sin((lat + dTheta) * degToRad) ;
            z = r*cos( (lat + dTheta) * degToRad ) ;
            glNormal3d( x, y, z ) ;
            glTexCoord2d(lon/360-0.25, (lat + dTheta-1)/(180)); 
            glVertex3d( x, y, z ) ;
            
            
            //Vertex 3
            x = r*cos((lon + dLon) * degToRad) * sin((lat) * degToRad) ;
            y = r*sin((lon + dLon) * degToRad) * sin((lat) * degToRad) ;
            z = r*cos((lat) * degToRad ) ;
            glNormal3d( x, y, z ) ;
            glTexCoord2d((lon + dLon)/(360)-0.25 ,(lat)/180);
            glVertex3d( x, y, z ) ;
            
            
            //Vertex 4
            x = r*cos((lon + dLon) * degToRad) * sin((lat + dTheta)* degToRad) ;
            y = r*sin((lon + dLon)* degToRad) * sin((lat + dTheta)* degToRad) ;
            z = r*cos((lat + dTheta)* degToRad ) ;
            glNormal3d( x, y, z ) ;
            glTexCoord2d((lon + dLon)/360-0.25, (lat + dTheta)/(180));
            glVertex3d( x, y, z ) ;
        }
        glEnd() ;        
    } // end sphere
}

///////////////////////////////////////////////////////////////////////
// GL and GLUT callbacks
///////////////////////////////////////////////////////////////////////
void glutDisplay()
{
    glvu.BeginFrame();
    glUseProgram(glslProgramHandle);
    
    
    //glClearColor(0.5, 0.5, 0.5, 0.0);
    //glClearColor(0.2, 0.2, 0.2, 0.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupShaders();
    
    // draw the textured quad
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);        glVertex2f(-2.0f, -2.0f);  // lower left
        glTexCoord2f(1.0f, 0.0f);        glVertex2f( 2.0f, -2.0f);  // lower right
        glTexCoord2f(1.0f, 1.0f);        glVertex2f( 2.0f,  2.0f);  // upper right
        glTexCoord2f(0.0f, 1.0f);        glVertex2f(-2.0f,  2.0f);  // upper left
    glEnd(); 
    
    // OR

    // textured teapot
    // the teapot includes texture coordinates (opposed to the glutSolidSphere or glutSolidTorus)
    //glutSolidTeapot(0.75);
    
    // OR
    
    // draw sphere with texture coordinates
    //drawSphere();
    
    //amplitude = sin(theta);
    
//    amplitude = sin(theta)*sin(theta*1.5*frequency);

    // simple motion
    //amplitude = sin(theta*1.5*frequency);
    
    //amplitude = sin(tan(cos(theta)*1.2));
    
    amplitude = sin(exp(cos(theta*0.8*frequency))*2.0);
    
    
    //drawAxes();
    glvu.EndFrame();
}

void runEverytime() {
    
    
    theta += 0.02;    
}

///////////////////////////////////////////////////////////////////////
// animate and display new result
///////////////////////////////////////////////////////////////////////
void glutIdle()
{
    if(animate){
        runEverytime();      
    }   
    
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
            
//        case '?':
//            cout << "help" << endl;
//            break;
            
        // motion            
        case 'a':
            animate = !animate;        
            break;
        
        // increase or decrease frequency
        case '-':
            frequency -= 0.1;
            cout << "frequency: " << frequency << endl;
            break;

        case '=':
            frequency += 0.1;
            cout << "frequency: " << frequency << endl;            
            break;            


            
        // cycle through patterns
        case '[':
            if (option >= 1.0)
                option -= 1.0;      
            break;
            
        case ']':
            option += 1.0;
            break;

        // scale pattern
        case ',':
            if (scale >= 1.0)
                scale -= 1.0;      
            break;
            
        case '.':
            scale += 1.0;
            break;
            
         // add more or less rings   
        case ';':
            if (nRings >= 1.0)
                nRings -= 1.0;
            break;

        case '\'':
            nRings += 1.0;
            break;
            

        // blotchyness
        case 'o':
            blotchyness -= 0.1;
            break;
            
        case 'p':
            blotchyness += 0.1;
            break;

            
            
        // pattern selectors
        case '0':
            option = 0.0;
            break;

        case '1':
            option = 1.0;
            break;

        case '2':
            option = 2.0;
            break;

        case '3':
            option = 3.0;
            break;

        case '4':
            option = 4.0;
            break;

        case '5':
            option = 5.0;
            break;

        //worley 
        case 'y':

            break;
            

            
            
        // r
        case 'e':
            colorVar.r -= 0.1;
            break;

        case 'r':
            colorVar.r += 0.1;
            break;
        
        // g
        case 's':
            colorVar.g -= 0.1;
            break;
            
        case 'd':
            colorVar.g += 0.1;
            break;
        
        // b
        case 'z':
            colorVar.b -= 0.1;
            break;
            
        case 'x':
            colorVar.b += 0.1;
            break;
            
            

            
        case 27:  // esc
                    
            break;
                     
        default:
            break;
    }
    
    // sets up different display modes for number keys
    //glvu.Keyboard(key,x,y);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutSpecial(int key, int x, int y)
{
    
    switch (key)
    {
        case GLUT_KEY_LEFT:
            shape -= 0.1; 
            cout << shape << endl;
            
            break;
        case GLUT_KEY_RIGHT:
            shape += 0.1;
            cout << shape << endl;
            
            break;
        case GLUT_KEY_UP:
            depth += 0.01;
            cout << depth << endl;
            
            break;
        case GLUT_KEY_DOWN:
           depth -= 0.01;            
            cout << depth << endl;
            
            break;
        default:
            break;
    }

}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutMouseClick(int button, int state, int x, int y)
{
    //glvu.Mouse(button,state,x,y);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void glutMouseMotion(int x, int y)
{
    //glvu.Motion(x,y);
}

//////////////////////////////////////////////////////////////////////////////
// open the GLVU window
//////////////////////////////////////////////////////////////////////////////
int glvuWindow()
{
    // setup GLUT window through glvu
    char title[] = "Procedural Textures";
    glvu.Init(title,
              GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH,
              0, 0, 1200, 700);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
//    glEnable(GL_LIGHTING);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_TEXTURE_2D);
//    glShadeModel(GL_SMOOTH);
//    glClearColor(0.5, 0.5, 0.5, 0.0);
    
    glEnable(GL_DEPTH_TEST);
    
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK); 
//    
    
    
    // create shader program
    // regular patterns
//    glslProgramHandle = createProgram("/Users/rjduranjr/Documents/UCSB/YEAR 2/f2012/594G/xcode/Final/Final/regularpatterns.vert", 
//                                      "/Users/rjduranjr/Documents/UCSB/YEAR 2/f2012/594G/xcode/Final/Final/regularpatterns.frag");
    
    // simplex noise
//    glslProgramHandle = createProgram("/Users/rjduranjr/code/594G/xcode/Final/Final/noise.vert", 
//                                      "/Users/rjduranjr/code/594G/xcode/Final/Final/noise.frag");

    // worley noise
    glslProgramHandle = createProgram("/Users/rjduran/UCSB/YEAR 2/f2012/594G/xcode/Final/Final/worley.vert",
                                      "/Users/rjduran/UCSB/YEAR 2/f2012/594G/xcode/Final/Final/worley.frag");
    

    
    
    
    
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
    
    return 0;
}
