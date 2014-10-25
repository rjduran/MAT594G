void setup() {
  size( 400, 400, P3D);
  smooth();
  background(0);
}

void draw() {
  background(0);
  //noFill();
  fill(0,255,255, 175);
  stroke(255);
  pushMatrix();
  translate(width/2, height/2);
  rotateX(0.02*mouseX);
  rotateY(0.02*mouseY);
  sphere(100);
  popMatrix();
}

void keyPressed() {
  // Catch the ESC key
  if ( keyCode == ESC ) { 
    key = 0;
  }
}

