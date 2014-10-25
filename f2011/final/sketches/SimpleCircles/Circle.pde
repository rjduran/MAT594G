class Circle {
 int x;
 int y;
 int r;
 
 Circle(int x, int y, int r) {
   this.x = x;
   this.y = y;
   this.r = r;
 } 
 
 void draw() {
   noStroke();
   fill(0);
   ellipse(x,y, r,r);
 }
 
 void updatePosition() {
   x = (int)random(0, 400);
   y = (int)random(0, 400);
 }
}
