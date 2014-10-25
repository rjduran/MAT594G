#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include "VEC3F.h"

using namespace std;

class Camera {	
public:
	VEC3F origin;
	VEC3F direction;
    
	Camera() {}	
	Camera(VEC3F o, VEC3F d) : origin(o), direction(d) {}
};

#endif