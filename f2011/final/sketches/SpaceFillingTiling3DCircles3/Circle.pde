class Circle {
  int x;
  int y;
  int z;
  float r;

  Circle(int x, int y, int z, float r) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.r = r;
  } 

  void draw() {
    //noStroke();
    //fill(0);
    //   ellipse(x,y, r,r);

    //noFill();
    fill(0, 255, 255, 175);
    stroke(255);
    pushMatrix();
    translate(x, y, z);
    //  rotateX(0.02*mouseX);
    //  rotateY(0.02*mouseY);
    sphere(r);
    
    // this adds another sphere to the edge of the object
    translate(r, r, r); 
    sphere(r);
    
    // this adds another sphere just offset but intersecting with previous sphere
    // maybe turn this into a bunpy sphere ball?
    float rr = r/4;
    translate(r-rr, r-rr, r-rr); 
    sphere(rr);
    
    popMatrix();
  }

  void setPosition(int x, int y, int z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
}

