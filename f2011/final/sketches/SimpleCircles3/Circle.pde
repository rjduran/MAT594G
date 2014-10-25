class Circle {
 int x;
 int y;
 float r;
 
 Circle(int x, int y, float r) {
   this.x = x;
   this.y = y;
   this.r = r;
 } 
 
 void draw() {
   noStroke();
   //fill(0);
   ellipse(x,y, r,r);
 }
 
 void setPosition(int x, int y) {
   this.x = x;
   this.y = y;
 }
}
