#ifndef TRIANGLE_H 
#define TRIANGLE_H

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#include "RAY.h"
#include "VEC3F.h"
#include <vector>
#include <iostream>



class TRIANGLE
{
public:
  TRIANGLE()
  {
    for (int x = 0; x < 3; x++)
      vertices[x] = texcoords[x] = normals[x] = NULL;
  };

  //////////////////////////////////////////////////////////////////////
  // IMPLEMENT ME
  //////////////////////////////////////////////////////////////////////
  double intersect(const RAY &ray, VEC3F& normal)
  {
	VEC3F x,v,w;

	// get all 3 triangle vertices
	VEC3F p0 = *vertices[0]; // vertex p0
	VEC3F p1 = *vertices[1]; // vertex p1
	VEC3F p2 = *vertices[2]; // vertex p2
	
	//calculate normal to triangle, any vertex will work since all points are in same plane
	normal = cross((p1-p0), (p2-p0));
	normal.normalize();
	
	//calculate t
	float t = -(((ray.origin - p0) * normal) / (ray.direction * normal));	

	// calculate the x value then check if it is within the triangle
	x = ray.origin + t*ray.direction;
	
	// flags to check intersection
	bool t0 = false;
	bool t1 = false;
	bool t2 = false; 
	
	// two parallel vectors point in the same direction if their dot product is positive
	if(cross(p1-p0, x-p0) * normal >= 0) {
		t0 = true;	
	} 

	if(cross(p2-p1, x-p1) * normal >= 0) {
		t1 = true;	
	} 

	if(cross(p0-p2, x-p2) * normal >= 0) {
		t2 = true;	
	} 
		
	if(t0 && t1 && t2) { // if they are all true, there is an intersection
		return t;
	} else {
		return -1;
	}	
  }

  VEC3F* vertices[3];
  VEC3F* texcoords[3];
  VEC3F* normals[3];
};

#endif
