//////////////////////////////////////////////////////////////////////////////////
// OBJ.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef OBJ_H
#define OBJ_H

#include <GLUT/glut.h>
#include <string>
#include <vector>
#include <map>
#include "VEC3F.h"
#include "TRIANGLE.h"

using namespace std;

class OBJ
{
public:
  OBJ() : _vertexArrayObject(0), _vertexBufferObject(0) {};
  OBJ(const string& filename) : _vertexArrayObject(0), _vertexBufferObject(0) { 
    load(filename); 
    normalize();
  };

	// return true if succeeded to read obj from fileName
	bool load(const string& filename);

  // draw the mesh to OpenGL
  void draw();

	// vertex positions
	vector<VEC3F> vertices;

	// vertex normals
	vector<VEC3F> normals;
	
  // vertex texture coordinates
	vector<VEC3F> texcoords;

  // mesh triangles
	vector<TRIANGLE> triangles;

  // manipulate the size and location of the mesh
  void normalize();
  void scale(VEC3F scaleVector);
  void translate(VEC3F translateVector);

private:
  string _filename;

  // upload to GPU handles
  GLuint _vertexArrayObject;
  GLuint _vertexBufferObject;
};

#endif
