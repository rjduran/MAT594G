/*//////////////////////////////////////////////////////////////////////  
  MAT594G Realistic Image Synthesis (Fall 2011)
  Course Website: http://mat.ucsb.edu/594G/
  Author: RJ Duran
  Email: rjduranjr@gmail.com (rjduran@mat.ucsb.edu)
  Date: 10/11/11
  Homework: #1

  //////////////////////////////////////////////////////////////////////*/

#include <VEC3F.h>
#include <sstream>

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
    }
}

float calcT(VEC3F oOrtho, VEC3F dOrtho, VEC3F c, float radius) {
	double root1, root2, result;
	double A,B,C, desc;
	
	A = dOrtho*dOrtho;
	B = 2*dOrtho*(oOrtho-c);
	C = (oOrtho-c)*(oOrtho-c) - pow(radius, 2);
	desc = pow(B, 2) - 4*A*C; 
	
	// check descriminant for roots
	// case 1: roots are imaginary, no intersection, return 0
	if (desc < 0.0) {		 		
		result = 0.0;
			
	// case 2: desc is positive, calculate possible solutions	
	// two roots, ray goes through sphere, use smaller result
	// one root, real, return -B/2*A
	} else { 
		root1 = 0.5*((-B + sqrt(desc)) / A);
		root2 = 0.5*((-B - sqrt(desc)) / A);
		
		if (root1 > root2) {								
			result = root2;
		} else {
			result = root1;
		}					
	}	
	return result;
}

// Ray Generation
VEC3F* generateRay(int i, int j, float left, float right, float bottom, float top, int nx, int ny, int eVar) {
	VEC3F dPersp;  		// d vector aka -w
	VEC3F oOrtho; 		// orthographic vector (image plane coords)	
	VEC3F e(0, 0, -1);
	double u, v, w, t;	
	
	// SPHERE
	VEC3F c(0, 0, 1); 	// sphere center
	float R = 0.25f;	// radius sphere		
	
	int mode = 1; // mode=0 (orthographic w/ flat image plane), 
				  // mode=1 (perspective w/ odd pixel "checker board" image plane)
	
	// calculate image plane coordinates for orthographic generation
	u = left + ((right-left)*(i + 0.5) / nx);   // image plane coordinate u
	v = bottom + ((top-bottom)*(j + 0.5) / ny); // image plane coordinate v					 			

	//     if(eVar*0.005 < 0.45) {
	// 	e[0] -= eVar*0.005; // change x coord to move left
	// } else {
	// 	e[0] += eVar*0.005; // change x coord to move right
	// }

//	std::cout << eVar*0.005 << std::endl;
//	e[2] += eVar*0.001; // change z coord eye location (if movie being made)

	 // change mode to set image plane coordinate w	
	// if(mode == 0) {	
	// 	w = 0.0; 
	// } else {
	// 	if(i%2 == 0 && j%2 == 0) {
	// 		w = -0.5;
	// 	} else {
	// 		w = 0.0;						 				
	// 	}		
	// }
	
	oOrtho = VEC3F(u, v, w); // ray origin
	dPersp = oOrtho - e; 	 // calculate dPerspective
		
	// calculate t
	t = calcT(e, dPersp, c, R);
			
	// return ray					
	return new VEC3F(e + t*dPersp);
}

bool intersectScene(VEC3F ray) {	
	bool onSphere;
	
	// check intersection with sphere surface
	if(ray[2] > 0) {
		onSphere = true;
	} else {
		onSphere = false;
	}	
	return onSphere;
}

VEC3F rayColor(VEC3F* ray) {	
	VEC3F colorRGB;
	VEC3F c(0, 0, 1); // sphere center 
	VEC3F n; 		  // normal vector
	
	VEC3F cr(1.0f, 1.0f, 1.0f); // surface color 
	VEC3F cl(1.0f, 1.0f, 1.0f); // light color
	VEC3F light(1, 1, -1); 		// light location
	VEC3F l;					// light direction
	
	if(!intersectScene(ray[0])) {
		//return black
		colorRGB = VEC3F(0.f, 0.f, 0.f);
	} else {		
		// return color								
		n = ray[0]-c;	   // calculate normal vector, p - c
		l = light-ray[0];  // calculate l vector, light - p
		
		// normalize n and l
		n.normalize();	
		l.normalize();				
		
		VEC3F crcl(cr[0]*cl[0], cr[1]*cl[1], cr[2]*cl[2]);		
		float nlDotProd = n*l; 		
		colorRGB = crcl*(nlDotProd);					
	}
		
	return colorRGB; // return vector with rgb values
}

//////////////////////////////////////////////////////////////////////
// Create an image and write it out to a file
//////////////////////////////////////////////////////////////////////
int main()
{
  int width = 400;
  int height = 400;
  VEC3F* image = new VEC3F[width * height]; // pixel values
  VEC3F* ray[width * height]; 				// array of VEC3F pointers
  int maxFrames = 1; 						// set to 30 to make 30 sequenced images
  
  for(int v = 0; v < maxFrames; v++) { 
	
    int i = 0; 
	for (int y = 0; y < height; y++) {
	  for (int x = 0; x < width; x++, i++) {
	    ray[i] = generateRay(x, y, -0.5f, 0.5f, -0.5f, 0.5f, width, height, v);  // generate Ray
		image[i] = rayColor(ray[i]); // get RGB pixel color of ray intersecting on surface
	  }
	}
	
	//save sequence of images for movie
	// std::stringstream stream;
	// stream << "image" << v << ".ppm";
	// std::string arraystring = stream.str();	
	// const char* filename = arraystring.c_str();
	
	//writePPM(image, width, height, filename);
	   } // v for loop
  writePPM(image, width, height, "image.ppm"); // comment this when saving movie
  delete[] image;

  return 0;
}


