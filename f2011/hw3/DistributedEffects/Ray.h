#ifndef _RAY_H_
#define _RAY_H_

#include <iostream>
#include "VEC3F.h"

using namespace std;

class Ray {	
public:
	VEC3F origin;
	VEC3F direction;
    
	Ray() {}	
	Ray(VEC3F o, VEC3F d) : origin(o), direction(d) {}
    
};

#endif