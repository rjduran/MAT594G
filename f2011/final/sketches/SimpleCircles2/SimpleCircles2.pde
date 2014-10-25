/*
This does something cloase to what we want but not correct. It searches through the circles 
 array to see what intersects with it. If a circle intersects with another circle the other circles position is 
 updated and checked next time around the draw loop. 
 
 */

Circle[] circles;
int nCircles;

float c, r;
int n;
float[] radii;
float canvasArea;

void setup() {
  size( 500, 500 );
  background(255);
  smooth();

  canvasArea = width*height;  
  c = 1.5;
  n = 50; // iterations
  //a = 30000; // intial area of shape
  //r = 100; // initial radius

  //nCircles = 100;
  radii = new float[n];
  circles = new Circle[n];
//  for (int i=0; i < nCircles; i++) {
//    circles[i] = new Circle((int)random(0, 400), (int)random(0, 400), 20);
//  }

  noStroke();
  noLoop(); // temp
}

void draw() {
  //background(255);

  // choose value of c
  //calculate initial area from Riemann zeta relationship
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

  println("canvasArea: " +canvasArea+ ", n: "+ n);

  float a = 0; // initial circle area
  float sum = 0;

  //calculate total area
  for (int i=0; i <= n; i++) {    
    if (i == 0) { 
      sum = 0;
    } 
    else {
      sum += pow(i, -c);
    }
  }
  a = canvasArea / sum; // calculated inital area
  println("a: "+ a);

  
  // calculate dimensions of object
  float totalArea = 0;
  for (int i=1; i < n; i++) {  
    float area; // area of object
  
    if (i == 0) { 
      area = a;
    } else {
      area = a * pow(i, -c);
    }

    float rad = sqrt(area/PI);
    //println( "area["+i+ "]: " + area +", radius: "+ rad );
    radii[i] = rad; // store all radius values
    //fill(random(0, 100));
    //circles[i] = new Circle((int)random(0, width), (int)random(0, height), rad);
    //ellipse(random(0, width), random(0, height), rad, rad);
    totalArea += area;
  }

  println("total area drawn: " + totalArea);
  
  // repeat
  //      choose a random position in the region of the plane being filled
  //      check for intersection of the new object at this position with all other objects
  //      if the new object does not intersect exit the repeat loop
  //   end repeat
  //   add the new new object to the plane

  
  // draw object

  //  for (int i=0; i < nCircles; i++) {
  //    for (int j=0; j < i; j++) {
  //
  //      if (i != j) { // dont check against itself
  //
  //          
  //          
  //          
  //        if ( separated(circles[i], circles[j]) ) {
  //           // if separate from all other circles, draw it
  //           
  //          circles[i].draw();
  //
  //
  //        } else {
  //        // set new position for circle, test again next time around
  //        circles[j].updatePosition();  
  //        }
  //          
  //      }
  //    }    
  //  }
}




boolean separated (Circle p1, Circle p2) {
  float dx, dy, dr;

  dx = p1.x - p2.x;
  dy = p1.y - p2.y;
  dr = sqrt(dx*dx + dy*dy);

  // if circles are separate more than their radaii, return true, else they intersect
  if (dr > p1.r + p2.r) {
    return true;
  } 
  else {
    return false;
  }
}

void keyPressed() {
  // Catch the ESC key
  if ( keyCode == ESC ) { 
    key = 0;
  }
}

