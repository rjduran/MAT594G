//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#include "obj.h"
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Normalize mesh to 1x1x1 cube centered at (0,0,0)
//////////////////////////////////////////////////////////////////////
void OBJ::normalize()
{
  VEC3F maxVert(vertices[0]);
  VEC3F minVert(vertices[0]);

  // get bounds
  for (unsigned int x = 0; x < vertices.size(); x++)
    for (int y = 0; y < 3; y++)
    {
      if (vertices[x][y] < minVert[y]) minVert[y] = vertices[x][y];
      if (vertices[x][y] > maxVert[y]) maxVert[y] = vertices[x][y];
    }

  VEC3F half(0.5, 0.5, 0.5);
  VEC3F diff = maxVert - minVert;
  double maxDiff = diff[0] > diff[1] ? diff[0] : diff[1];
  maxDiff = maxDiff > diff[2] ? maxDiff : diff[2];
  for (unsigned int x = 0; x < vertices.size(); x++)
  {
    vertices[x] -= minVert;
    vertices[x] *= 1.0 / maxDiff;
    vertices[x] -= half;
  }
}

//////////////////////////////////////////////////////////////////////
// scale the whole mesh
//////////////////////////////////////////////////////////////////////
void OBJ::scale(VEC3F scaleVector)
{
  for (unsigned int x = 0; x < vertices.size(); x++)
  {
    vertices[x][0] = vertices[x][0] * scaleVector[0];
    vertices[x][1] = vertices[x][1] * scaleVector[1];
    vertices[x][2] = vertices[x][2] * scaleVector[2];
  }
}

//////////////////////////////////////////////////////////////////////
// translate the whole mesh
//////////////////////////////////////////////////////////////////////
void OBJ::translate(VEC3F translateVector)
{
  for (unsigned int x = 0; x < vertices.size(); x++)
    vertices[x] += translateVector;
}

//////////////////////////////////////////////////////////////////////
// read an obj file using stdlib
//////////////////////////////////////////////////////////////////////
bool OBJ::load(const string& filename)
{
	// clear anything that existed before
	vertices.resize (0);
	normals.resize (0);

  // make a copy of the filename
  _filename = string(filename);
	
	// open up file
  FILE* file = fopen(filename.c_str(), "r");
	
	if (file == NULL)
	{
		cerr << "Can't read input file " << filename << endl;
		return false;
	}

	// read through line by line
	int lineNumber = 0;
	bool faceSeen = false;
  while (true)
	{
		if (feof(file)) 
      break;

		char type[1024];
		lineNumber++;
    fscanf(file, "%s", type);

		if (feof(file) || ferror(file)) break;

		// reading vertices
		if (strcmp(type, "v") == 0)
		{
      if (faceSeen)
      {
        cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " : " << endl;
        cout << " Error! Vertex seen after face read begun!" << endl;
      }

			VEC3F v;
			readVertex(file, v);
			vertices.push_back(v);
		}

		// vertex normals
		if (strcmp(type, "vn") == 0)
		{
			VEC3F vn;
      readVertex(file, vn);
			normals.push_back(vn);
		}

		// vertex texcoords
		if (strcmp(type, "vt") == 0)
		{
			VEC3F vt;
      readVertex(file, vt);
			texcoords.push_back(vt);
        }

		// reading triangles
	  TRIANGLE f;
		if (type[0] == 'f')
		{
      char indices[256];
      faceSeen = true;
      
      for (int x = 0; x < 3; x++)
      {
        int totalChars = fscanf(file, "%s", indices); 
        if (feof(file) || ferror(file)) break;

        int vertexIndex = -1;
        int texcoordIndex = -1;
        int normalIndex = -1;
        char* texcoordExists = strchr(indices, '/');
        char* normalExists   = strrchr(indices, '/');

        int vertexEnd = texcoordExists - indices;
        int texcoordEnd = normalExists - indices;

        // extract the vertex index
        if (texcoordExists == NULL)
        {
          // if there is nothing but an index
          vertexIndex = atoi(indices);
        }
        else
        {
          // get the right substring
          char vertexString[256];
          strncpy(vertexString, indices, vertexEnd);

          // convert it to an int
          vertexIndex = atoi(vertexString);
        }

        // extract the texture index
        if (texcoordExists)
        {
          if (normalExists == NULL)
          {
            // extract to the end of the string
            char texcoordString[256];
            strncpy(texcoordString, &indices[vertexEnd + 1], totalChars - vertexEnd);
            
            // convert it to an int
            texcoordIndex = atoi(texcoordString);
          }
          else
          {
            // extract to the beginning of the normal index
            char texcoordString[256];
            strncpy(texcoordString, &indices[vertexEnd + 1], texcoordEnd - vertexEnd);
            
            // convert it to an int
            texcoordIndex = atoi(texcoordString);
          }
        }

        // extract the normal index
        if (normalExists)
        {
          // extract to the end of the string
          char normalString[256];
          strncpy(normalString, &indices[texcoordEnd + 1], totalChars - texcoordEnd);
            
          // convert it to an int
          normalIndex = atoi(normalString);
        }

        // subtract one and store
        f.vertices[x] = &vertices[vertexIndex - 1];
        f.texcoords[x] = &texcoords[texcoordIndex - 1];
        f.normals[x] = &normals[normalIndex - 1];
      }

      if (feof(file) || ferror(file)) break;

		  // store the quad
		  triangles.push_back(f);
		}
	}

  fclose(file);
  cout << filename.c_str() << " successfully loaded" << endl;
	return true;
}

//////////////////////////////////////////////////////////////////////
// read in a vertex
//////////////////////////////////////////////////////////////////////
void OBJ::readVertex(FILE* file, VEC3F& vec)
{
  fscanf(file, "%f %f %f", &(vec[0]), &(vec[1]), &(vec[2]));
}
