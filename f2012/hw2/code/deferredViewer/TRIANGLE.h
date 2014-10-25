#ifndef TRIANGLE_H 
#define TRIANGLE_H

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#include <vector>

#if _WIN32
#include <gl/glut.h>
#elif __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class TRIANGLE
{
public:
  TRIANGLE()
  {
    for (int x = 0; x < 3; x++)
      vertices[x] = texcoords[x] = normals[x] = NULL;
  };


  // draw this triangle to GL
  void draw()
  {
    // recompute normals every time
    VEC3F normal = VEC3F::cross(*vertices[1] - *vertices[0], 
                                *vertices[2] - *vertices[0]);
    normal.normalize();

    glBegin(GL_TRIANGLES);
      glNormal3f(normal[0], normal[1], normal[2]);
      glVertex3fv(*vertices[0]);
      glVertex3fv(*vertices[1]);
      glVertex3fv(*vertices[2]);
    glEnd();
  }

  VEC3F* vertices[3];
  VEC3F* texcoords[3];
  VEC3F* normals[3];
};

#endif
