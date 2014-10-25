#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <iostream>

#include "VEC3F.h"

class Light {
public:
	VEC3F origin;	// light location
	VEC3F color;	// light color
	VEC3F specular; // specular color
    
	Light(){}			
};

#endif