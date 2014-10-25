#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>

#include "VEC3F.h"

class Light {
public:
	VEC3F origin;	// light location
	VEC3F color;	// light color
	VEC3F specular; // specular color
	int width, height;
	
    // spot light
	Light() : origin(VEC3F(0.0f, 0.0f, 0.0f)), color(1.0f, 1.0f, 1.0f) {
		width = 1;
		height = 1;
	}			

	// area light
	Light(int w, int h) : origin(VEC3F(0.0f, 0.0f, 0.0f)), color(1.0f, 1.0f, 1.0f) {
		width = w;
		height = h;				
	}				
};

#endif