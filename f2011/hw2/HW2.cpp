/*//////////////////////////////////////////////////////////////////////  
  MAT594G Realistic Image Synthesis (Fall 2011)
  Course Website: http://mat.ucsb.edu/594G/
  Author: RJ Duran
  Email: rjduranjr@gmail.com (rjduran@mat.ucsb.edu)
  Date: 10/20/11
  Homework: #2

  //////////////////////////////////////////////////////////////////////*/
#include <math.h>
#include <VEC3F.h>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

#define IMG_WIDTH 400
#define IMG_HEIGHT 400

//////////////////////////////////////////////////////////////////////
// Objects
//////////////////////////////////////////////////////////////////////
class Ray {	
  public:
	VEC3F origin;
	VEC3F direction;
		
	Ray() {}	
	Ray(VEC3F o, VEC3F d) : origin(o), direction(d) {}
};

class Light {
  public:
	VEC3F origin;	// light location
	VEC3F color;	// light color
	VEC3F specular; // specular color
		
	Light(){}			
};

class Surface {
  public:
	virtual bool hit(VEC3F* ray) {
		return false;
	}
};

class Sphere : public Surface {
  public:	
	VEC3F color; // surface color
	VEC3F center; // center point position
	float radius; // radius from center
	float t; // t calculated
	string material; // material for object
	
	Sphere() : color(0.f, 0.f, 0.f), material("SOLID") {}
	
	float hit(Ray ray) {
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

//////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////

Ray* rayObj[IMG_WIDTH * IMG_HEIGHT]; // array of pointers to Ray objects

vector<Sphere> spheres, hitSphereObjects;	
vector<Light> lights;	

Sphere* sphereHit;
int depthCtrMirror = 0;
int depthCtrGlass = 0;

//////////////////////////////////////////////////////////////////////
// Write out a PPM file of dimensions width x height to 'filename'
//////////////////////////////////////////////////////////////////////
void writePPM(VEC3F* image, int width, int height, const char* filename)
{
  // Dump to PPM
  FILE *f = fopen(filename, "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
      int i = x + (height - 1 - y) * width;

      // map to the [0,255] range
      int red   = image[i][0] * 255;
      int green = image[i][1] * 255;
      int blue  = image[i][2] * 255;
      fprintf(f,"%d %d %d ", red, green, blue);
	// cout << "["<< red << ", " << green << ", " << blue << "]" << endl; 
    }
}

// Ray Generation
// Convert (i,j) into image plane coordinates
Ray* generateRay(int i, int j, int eVar) {
	int nx = IMG_WIDTH;
	int ny = IMG_HEIGHT;
		
	// image plane dimensions
	float left = -0.5f;
	float right = 0.5f;
	float top = 0.5f;
	float bottom = -0.5f;
	
	// image plane coordinates
	double u, v, w;

	VEC3F rayDirection;//(0,0,1); // d vector aka -w is positive forward		
	VEC3F rayOrigin; // orthographic vector origin
	VEC3F eyePosition(0, 0, -1);  // eye position
			
	// calculate image plane coordinates for orthographic generation
	u = left + ((right-left)*(i + 0.5) / nx);   // image plane coordinate u
	v = bottom + ((top-bottom)*(j + 0.5) / ny); // image plane coordinate v					 			
	w = 0.0; 
	
	//eyePosition[2] += eVar*0.01; // change z coord eye location (if movie being made)
	
	rayOrigin = VEC3F(u, v, w); // ray origin (rays generated at image plane coordinates u, v, w)	 
	rayDirection = rayOrigin - eyePosition; // calculate distance between eye position and ray origin at image plane

	return new Ray(rayOrigin, rayDirection);	
}

float intersectScene(Ray ray, Sphere *sphereIn) {	
	float t = -1.0f;
		
	for(int i=0; i < (int)spheres.size(); i++ ) {
//		if(sphereIn == &(spheres[i])) { cout << "hit myself" << endl; } // debug

		if(sphereIn != &spheres[i]) {
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

		Sphere mySphere = *sphereHit; // grab the Sphere hit to get center value
		VEC3F intersectPoint = ray.origin + t*ray.direction; // caluclate the intersection point on the surface
		VEC3F n = intersectPoint - mySphere.center;	// calculate normal vector to surface of sphere, p - c
		n.normalize(); // do first thing		
						
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
				
				colorRGB = VEC3F(0.f, 0.f, 0.f);
				
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
					
//				} 
// 				else if(mySphere.material == "GLASS") {	
// 					if (depthCtrGlass < 1) { // 1 level of recursion max
// 						++depthCtrGlass;
// 						
// 						//////						
// 						Ray refractedRay;
// 						
// 						// generate refracted ray
// 						// origin and direction
// 						VEC3F d = ray.origin - intersectPoint; // from ray origin (eye) to intersect point
// 						d.normalize();
// 
// 						// to get theta, the angle between d and n, we take the arccos(d*n), both normalized						
// 						float costheta = d*n;
// 						float theta = acos(d*n); // cos() = d*n 
// 						float sinetheta = sin(theta);						
// 						float phi = asin(1.5*sin(theta));						
// 						
// 						// into glass form air													
// //						if(phi < theta) { 
// 							refractedRay = generateRefractedRay(1.5f, theta, d, n, intersectPoint);
// 
// //						} else {
// 						// coming out of glass
// //							refractedRay = generateRefractedRay(1/1.5f, theta, d, n, intersectPoint);
// //						}
// 												 
// 						// // total internal reflection
// 						if(true) {
// 							// return black
// 							colorRGB = VEC3F(0.f, 0.f, 0.f);
// 						} else {
// 							colorRGB += rayColor(refractedRay);				
// 						}
// 						
// 						
// 					} else {
// 						depthCtrGlass = 0;
// 					}						
// 																						
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

//////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////
int main() {
	VEC3F* image = new VEC3F[IMG_WIDTH * IMG_HEIGHT]; // pixel values
	//  int maxFrames = 1; // set to 30 to make 30 sequenced images

	// setup scene
	int nSpheres = 5;
	int nLights = 2;
	
	// setup objects
	spheres.resize(nSpheres);
	lights.resize(nLights);

	// glass
	spheres[0].center = VEC3F(0, -0.25, 1);
	spheres[0].radius = 0.25f;
	spheres[0].material = "GLASS";	
	
	// right
	spheres[1].center = VEC3F(0.255, -0.25, 2);
	spheres[1].color = VEC3F(1.0f, 0.0f, 0.0f);
	spheres[1].radius = 0.25f;

	// left
	spheres[2].center = VEC3F(-0.255, -0.25, 2);
	spheres[2].radius = 0.25f;
	spheres[2].material = "MIRROR";
		
	// middle
	spheres[3].center = VEC3F(0, -0.25, 3);
	spheres[3].color = VEC3F(0.0f, 1.0f, 0.0f);
	spheres[3].radius = 0.25f;

	// big sphere
	spheres[4].center = VEC3F(0, -10001, 1);
	spheres[4].color = VEC3F(1.0f, 1.0f, 1.0f);
	spheres[4].radius = 10000.5f;

	// light1
	lights[0].origin = VEC3F(50, 50, -10);  // light location	
	lights[0].color = VEC3F(0.5f, 0.5f, 0.5f);  // light color
	lights[0].specular = VEC3F(1.0f, 1.0f, 1.0f);  // specular color

	// light2
	lights[1].origin = VEC3F(-50, 50, -10);  // light location	
	lights[1].color = VEC3F(0.5f, 0.5f, 0.5f);  // light color
	lights[1].specular = VEC3F(1.0f, 1.0f, 1.0f);  // specular color

//  for(int v = 0; v < maxFrames; v++) { 
	
	// Loop through each pixel in image
    int i = 0; 
	for (int y = 0; y < IMG_HEIGHT; y++) {
	  for (int x = 0; x < IMG_WIDTH; x++, i++) {
		rayObj[i] = generateRay(x, y, 0);  // generate Ray object
//		cout << "rayObj[" << i << "]: " << endl;	
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
	
  writePPM(image, IMG_WIDTH, IMG_HEIGHT, "image.ppm"); // comment this when saving movie
  delete[] image;

  return 0;
}


