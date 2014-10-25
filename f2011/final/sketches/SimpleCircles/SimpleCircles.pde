/*
This does something cloase to what we want but not correct. It searches through the circles 
array to see what intersects with it. If a circle intersects with another circle the other circles position is 
updated and checked next time around the draw loop. 

*/

Circle[] circles;
int nCircles;

void setup() {
  size( 400, 400 );
  background(255);
  smooth();
  nCircles = 10;

  circles = new Circle[nCircles];

  for (int i=0; i < nCircles; i++) {
    circles[i] = new Circle((int)random(0, 400), (int)random(0, 400), 40);
  } 

}

void draw() {
  background(255);

  for (int i=0; i < nCircles; i++) {
    for (int j=0; j < nCircles; j++) {

      if (i != j) { // dont check against itself

        if ( separated(circles[i], circles[j]) ) {
           // if separate from all other circles, draw it

          circles[i].draw();
        } else {
        // set new position for circle, test again next time around
        //circles[j].updatePosition();  
        }
          
      }
    }    
  }
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

