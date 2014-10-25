import processing.opengl.*;

import peasy.*;

PeasyCam cam;
Circle[] circles;
int nCircles;

float c;
int n;
float[] radii;
float canvasArea;

void setup() {
  size( 800, 800, OPENGL);
  int W = width;
  int H = height;
  int D = width; // depth is the same as width if square
  cam = new PeasyCam(this, width/2, height/2, 0, 1000); // point camera at center of volume
  cam.setMinimumDistance(50);
  cam.setMaximumDistance(2000);

  //background(0);
  smooth();

  canvasArea = W*H;  // overall sketch volume
  c = 1.4; // choose value of c to adjust packing
  n = 50; // number of iterations (circles)

  radii = new float[n];
  circles = new Circle[n];

  float a = calcInitialArea(canvasArea); // calculate a
  calcDimensions(a);  // calculate radii for all objects

  // main loop
  for (int i=0; i < n; i++) { 
    int x, y, z;
    boolean drawCircle = true;

    do {
      drawCircle = true;
      // choose random position for circle
      x = (int)random(0, width);
      y = (int)random(0, height);

      for (int j=0; j < i; j++) {

        if ( dist(x, y, circles[j].x, circles[j].y) < radii[j]+radii[i] ) { // intersection, dont draw it. distance between new point and previous point is smaller than radius
          drawCircle = false;
        } // else, no intersection, draw it
      }
    } 
    while ( !drawCircle );

    circles[i] = new Circle(x, y, 0, radii[i]); // add circle
  }
}

void draw() {
  background(0);
  // main loop
  for (int i=0; i < n; i++) { 

    circles[i].draw(); // when it comes out it will be drawn in the right place
  }
}

float calcInitialArea(float cArea) {
  float a = 0; // initial circle area
  float sum = 0;

  // calculate initial area from Riemann zeta relationship
  for (int i=0; i <= n; i++) {    
    if (i == 0) { 
      sum = 0;
    } 
    else {
      sum += pow(i, -c);
    }
  }
  a = (float)cArea / sum; // calculated inital area
  println("cArea: "+cArea+ ", a: "+ a);  
  return a;
}

void calcDimensions(float a) {
  // calculate radius of each circle
  float totalArea = 0;
  for (int i=0; i < n; i++) {  
    float area; // calculate dimensions of object
    if (i == 0) { 
      area = 0;
    } 
    else {
      area = a * pow(i, -c);
    }
    radii[i] = sqrt(area/PI); // store all radius values
    totalArea += area;
  }
  println("total area filled: " + totalArea);
}

