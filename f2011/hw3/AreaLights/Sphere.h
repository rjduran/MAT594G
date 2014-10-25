#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <iostream>
#include <string>

#include "VEC3F.h"
#include "Surface.h"
#include "Ray.h"

using namespace std;

class Sphere : public Surface {
public:	
	class VEC3F color; // surface color
	class VEC3F center; // center point position
	float radius; // radius from center
	float t; // t calculated
	string material; // material for object
	
	Sphere() : color(0.f, 0.f, 0.f) {}
	
	float hit (Ray ray) {
		double root1, root2, result;
		double A,B,C, desc;
		
		t = 0.0;
		
		A = ray.direction * ray.direction;
		B = 2*ray.direction*(ray.origin-center);
		C = (ray.origin-center)*(ray.origin-center) - pow(radius, 2);
		desc = pow(B, 2) - 4*A*C; 
		// check descriminant for roots
		// case 1: roots are imaginary, no intersection, return 0
		if (desc < 0.0) {		 		
			result = -1;
            
            // case 2: desc is positive, calculate possible solutions	
            // two roots, ray goes through sphere, use smaller result
            // one root, real, return -B/2*A
		} else { 
			root1 = 0.5*((-B + sqrt(desc)) / A);
			root2 = 0.5*((-B - sqrt(desc)) / A);
			
			if(root1 < 0 && root2 < 0) { // both are negative
				result = -1;
			} else if(root1 >= 0 && root2 < 0) { // root1 is positive, root2 is neg
				result = root1;
			} else if(root2 >= 0 && root1 < 0) { // root2 is positive, root1 is neg
				result = root2;
			} else { // both are positive
				if(root1 > root2) { 
					result = root2; // root2 < root1
				} else {
					result = root1; // root 1 < root2
				}
			}									
		}	
		
		t = result;						
		return result;
	}				
};

#endif