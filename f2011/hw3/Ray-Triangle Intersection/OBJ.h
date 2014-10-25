//////////////////////////////////////////////////////////////////////////////////
// OBJ.cpp
//////////////////////////////////////////////////////////////////////////////////

#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>
#include <map>
#include "VEC3F.h"
#include "TRIANGLE.h"

using namespace std;

class OBJ
{
public:
	// return true if succeeded to read obj from fileName
	bool load(const std::string& filename);

  void normalize();

	// vertex positions
	std::vector<VEC3F> vertices;

	// vertex normals
	std::vector<VEC3F> normals;
	
  // vertex texture coordinates
	std::vector<VEC3F> texcoords;

  // mesh triangles
	std::vector<TRIANGLE> triangles;

  void scale(VEC3F scaleVector);
  void translate(VEC3F translateVector);

private:
  string _filename;

  // read in a vertex
  void readVertex(FILE* file, VEC3F& vec);
};

#endif
