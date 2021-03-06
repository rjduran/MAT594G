
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "VEC3F.h"
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "ImageWrite.h"
#include "Camera.h"

using namespace std;

// Global
#define IMG_WIDTH 400
#define IMG_HEIGHT 400

Ray* rayObj[IMG_WIDTH * IMG_HEIGHT]; // array of pointers to Ray objects

vector<Sphere> spheres;	
vector<Light> lights;
Camera camera;	

Sphere* sphereHit;
int depthCtrMirror;
int depthCtrGlass;

// Ray Generation
// Convert (i,j) into image plane coordinates
Ray* generateRay(int i, int j) {
	int nx = IMG_WIDTH;
	int ny = IMG_HEIGHT;
    
	// image plane dimensions
	float left = -0.5f;
	float right = 0.5f;
	float top = 0.5f;
	float bottom = -0.5f;
	
	// image plane coordinates
	double u, v, w;
    
	VEC3F rayDirection;//(0,0,1); // -w is positive forward		
	VEC3F rayOrigin; // vector origin
//	VEC3F eyePosition(0,0,-100); // eye position is far back from image plane. this works
	VEC3F eyePosition(0,0,-100); 
		
	// calculate image plane coordinates
	// place image plane at focal length
	u = left + ((right-left)*(i + 0.5) / nx);   // image plane coordinate u
	v = bottom + ((top-bottom)*(j + 0.5) / ny); // image plane coordinate v					 			
	w = 0.0; // move image plane
	
	
		
	//cout << camera.origin <<  endl;
	
	rayOrigin = VEC3F(u, v, w); // ray origin (rays generated at image plane coordinates u, v, w)	 
	rayDirection = rayOrigin - eyePosition; // calculate distance between eye position and ray origin at image plane

	// rayOrigin = camera.origin; // ray origin at camera position
	// rayDirection = rayOrigin - VEC3F(u, v, w); // direction towards point on image plane
	return new Ray(rayOrigin, rayDirection);	
}

float intersectScene(Ray ray, Sphere *sphereIn) {	
	float t = -1.0f;
    
	// loop through all spheres in scene
	for(int i=0; i < (int)spheres.size(); i++ ) {
        //		if(sphereIn == &(spheres[i])) { cout << "hit myself" << endl; } // debug
        
		// check if sphere hit's itself, if it doesn't, check if it hits anything
		if(sphereIn != &spheres[i]) {

			// check if sphere is hit with ray
			if((t = spheres[i].hit(ray)) >= 0) { 
				sphereHit = &spheres[i]; // save reference to address of hit sphere
				break; // break out, return t
			} 
		}		
	}
	return t;
}

VEC3F rayColor(Ray ray) {	
	VEC3F colorRGB; //  color value
	VEC3F finalColor;
    
	float t; // t value for intersected point
    
	// check if the scene is intersected by a ray	
	if((t = intersectScene(ray, NULL)) >= 0) {		
        // if we are here, we hit something

		Sphere mySphere = *sphereHit; // grab the Sphere hit to get center value		
		VEC3F intersectPoint = ray.origin + t*ray.direction; // caluclate the intersection point on the surface
		VEC3F n = intersectPoint - mySphere.center;	// calculate normal vector to surface of sphere, p - c
		n.normalize(); // do first thing at intersection point		
        
		// before calculating color, determine if it should have a 
		// color in the first place by shooting a shadow ray
		// from the hit point towards the light source(s)
		VEC3F shadowRayOrigin = intersectPoint;
		shadowRayOrigin += 0.01*n; // point offset to avoid self intersection			
        
		for(int k=0; k < (int)lights.size(); k++) {
            
			// generate a shadow ray to shoot at light source(s)					 	
			VEC3F shadowRayDirection = lights[k].origin - intersectPoint;
			shadowRayDirection.normalize();
			
			Ray shadowRay(shadowRayOrigin, shadowRayDirection);
			
			if(intersectScene(shadowRay, &mySphere) >= 0) {
				// the shadow ray intersected something if we are in here
				// set color value for pixel to black since no light is actually
				// going there
				// 
				// determine which sphere is hit with ray
				// look at its t value to make sure it's smaller than
				// the distance to the light source but greater than 0
								
				colorRGB += VEC3F(0.f, 0.f, 0.f);
				
			} else {
                
				VEC3F cl = lights[k].color; // light color	
				VEC3F lpos = lights[k].origin; // light direction		
				VEC3F cs = lights[k].specular; // specular color
                
				VEC3F ca(1.0f, 0.0f, 0.0f);  // ambient term
				
				// calculate color of point on object
				// 				
				VEC3F cr = mySphere.color; // sphere color				
                
				VEC3F l = lpos-intersectPoint;  	// calculate l vector from light source to point on sphere, light - p
				l.normalize();				
                
				// ambient term
				VEC3F ambient(ca[0]*cl[0], ca[1]*cl[1], ca[2]*cl[2]); // pointwise product
                
				// lambertian term (diffuse)
				VEC3F crcl(cr[0]*cl[0], cr[1]*cl[1], cr[2]*cl[2]);		
				VEC3F lambertian = crcl*max(0.0f, n*l);					
                
				// phong term (specular)
				VEC3F cscl(cs[0]*cl[0], cs[1]*cl[1], cs[2]*cl[2]);		
				VEC3F rPhong = -l+2*n*(l*n);
				VEC3F vPhong = ray.origin - intersectPoint; // vector from intersection point towards ray origin (eye)
				vPhong.normalize();
				rPhong.normalize();	
				VEC3F phong = cscl*powf(max(0.0f, vPhong * rPhong), 10);
                
				// check material parameter
				if(mySphere.material == "MIRROR") {					
					if (depthCtrMirror < 1) { // 1 level of recursion max
						++depthCtrMirror;
                        
						VEC3F dMirror = ray.origin - intersectPoint; // from ray origin (eye) to intersect point
						dMirror.normalize();
                        
						VEC3F rMirror = -dMirror + 2*n*(dMirror*n); 
						rMirror.normalize();
                        
						VEC3F reflectionRayOrigin = intersectPoint;
						reflectionRayOrigin += 0.01*n; // point offset to avoid self intersection			
                        
						Ray reflectionRay(reflectionRayOrigin, rMirror); // generate reflection ray
						colorRGB += rayColor(reflectionRay);				
					} else {
						depthCtrMirror = 0;
					}
																										
				} else {
					// no material, just solid object
					colorRGB += lambertian + phong; // sum the total over each light					
				}
                
				// fix color wrap around
				if (colorRGB[0] > 1.0f) { colorRGB[0] = 1.0f; }
				if (colorRGB[1] > 1.0f) { colorRGB[1] = 1.0f; }
				if (colorRGB[2] > 1.0f) { colorRGB[2] = 1.0f; }
                
			} // end of shadow checking
            
		} // end of for light loop
		
		// sum color values for each light source
		finalColor += colorRGB;
		
	} else {
		// nothing was hit, set to black
		finalColor = VEC3F(0.f, 0.f, 0.f);
	}
    
	return finalColor; // return final color value as (r,g,b)
}


// Main
int main () {
    ImageWrite iw;
    VEC3F* image = new VEC3F[IMG_WIDTH * IMG_HEIGHT]; // pixel values
	
	// depth counters
    depthCtrMirror = 0;
    depthCtrGlass = 0;
    
	// setup scene
	int nSpheres = 11;
	int nLights = 1;
	
	// setup objects
	spheres.resize(nSpheres);
	lights.resize(nLights);
	
	// setup camera
	camera.origin = VEC3F(0, 0, -4);
	camera.direction = VEC3F(0, 0, 1);	
    
//
//

	for(int x=0; x < nSpheres-1; x++) {
	
		spheres[x].center = VEC3F(0,0,0) + VEC3F(0.06,0,30) * (5-x);
		cout <<  spheres[x].center << endl;
		spheres[x].radius = 0.05f;
		spheres[x].color = VEC3F(1.0f, 0.0f+0.15*x, 0.0f);	
	}
	
	// (-5, 0, 15)
	// (-4, 0, 12)
	// (-3, 0, 9)
	// (-2, 0, 6)
	// (-1, 0, 3)
	// (0, 0, 0)
	// (1, 0, -3)
	// (2, 0, -6)
	// (3, 0, -9)
	// (4, 0, -12)

	// spheres[0].center = VEC3F(0, 0, 100);
	// spheres[0].color = VEC3F(0.0f, 1.0f, 0.0f);
	// spheres[0].radius = 0.05f;
			       
	// ground sphere
	spheres[11].center = VEC3F(0, -1e4, 0);
	spheres[11].color = VEC3F(1.0f, 1.0f, 1.0f);
	spheres[11].radius = 1e4 - 0.05;
	
	// light
	lights[0].origin = VEC3F(8, 81.6, 8);  // light location	
	lights[0].color = VEC3F(1.0f, 1.0f, 1.0f);  // light color
	lights[0].specular = VEC3F(1.0f, 1.0f, 1.0f);  // specular color

////////////////////////////////////////////////////////////////////////
// WORKING
////////////////////////////////////////////////////////////////////////
/*
	spheres[0].center = VEC3F(0, -0.25, 3);
	spheres[0].color = VEC3F(0.0f, 1.0f, 0.0f);
	spheres[0].radius = 0.25f;

	// big sphere
	spheres[1].center = VEC3F(0, -10001, 1);
	spheres[1].color = VEC3F(1.0f, 1.0f, 1.0f);
	spheres[1].radius = 10000.5f;

	// light1
	lights[0].origin = VEC3F(50, 50, -10);  // light location	
	lights[0].color = VEC3F(1.0f, 1.0f, 1.0f);  // light color
	lights[0].specular = VEC3F(1.0f, 1.0f, 1.0f);  // specular color    
*/	
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

    //  for(int v = 0; v < maxFrames; v++) { 

	// Loop through each pixel in image
    int i = 0; 
	for (int y = 0; y < IMG_HEIGHT; y++) {
        for (int x = 0; x < IMG_WIDTH; x++, i++) {
            rayObj[i] = generateRay(x, y);  // generate Ray object
            image[i] = rayColor(*rayObj[i]); // get RGB pixel color of ray intersecting on surface
        }
	}
	
	//save sequence of images for movie
	// std::stringstream stream;
	// stream << "image" << v << ".ppm";
	// std::string arraystring = stream.str();	
	// const char* filename = arraystring.c_str();
	// 
	// writePPM(image, IMG_WIDTH, IMG_HEIGHT, filename);
	// cout << filename << " saved" << endl;
	//    } // v for loop
	
    iw.writePPM(image, IMG_WIDTH, IMG_HEIGHT, "image.ppm"); // comment this when saving movie
    delete[] image;
    
    return 0;
}

