#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <iostream>
#include "VEC3F.h"

class Surface {
public:
	virtual bool hit (VEC3F* ray) {
		return false;
	}
};

#endif