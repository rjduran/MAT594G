#ifndef RAY_H
#define RAY_H

#include "VEC3F.h"

class RAY 
{
public:
  VEC3F origin, direction; 
  RAY(VEC3F origin_, VEC3F direction_) : origin(origin_), direction(direction_) {} 
};

#endif
