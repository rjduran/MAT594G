/*
This does something cloase to what we want but not correct. It searches through the circles 
 array to see what intersects with it. If a circle intersects with another circle the other circles position is 
 updated and checked next time around the draw loop. 
 
 */

Circle[] circles;
int nCircles;

float c;
int n;
float[] radii;
float canvasArea;

void setup() {
  size( 1000, 1000 );
  background(255);
  smooth();

  canvasArea = width*height;  // overall sketch area
  c = 1.4; // choose value of c
  n = 1000; // iterations

  radii = new float[n];
  circles = new Circle[n];

  float a = calcInitialArea(canvasArea/4); // calculate a
  calcDimensions(a);  // calculate radii for all objects

  noStroke();
  noLoop(); // temp
}

void draw() {
  //background(255);

  //initiate random number generator

  //repeat for i=0 to some chosen number of iterations n

  //   area of new object = initial area multiplied by pow(i,-c)
  //   calculate the dimensions of the new object given the area of the new object
  //   repeat
  //      choose a random position in the region of the plane being filled
  //      check for intersection of the new object at this position with all other objects
  //      if the new object does not intersect exit the repeat loop
  //   end repeat
  //   add the new new object to the plane

  //end repeat  
  
  translate(width/4, height/4);
  // main loop
  for (int i=0; i < n; i++) { 
    int x, y;
    boolean drawCircle = true;
    
    do {
      drawCircle = true;
      // choose random position for circle
      x = (int)random(0, width/2);
      y = (int)random(0, height/2);

      for (int j=0; j < i; j++) {

        if ( dist(x, y, circles[j].x, circles[j].y) < radii[j]+radii[i] ) { // intersection, dont draw it. distance between new point and previous point is smaller than radius
          drawCircle = false;
        } // else, no intersection, draw it
      }
    } 
    while ( !drawCircle );

    circles[i] = new Circle(x, y, radii[i]*2); // add circle

    fill(0, 50+(i*3));
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
    //float rad = sqrt(area/PI); // get radius
    radii[i] = sqrt(area/PI); // store all radius values
    totalArea += area;
  }
  println("total area drawn: " + totalArea);

  //return rad;
}

