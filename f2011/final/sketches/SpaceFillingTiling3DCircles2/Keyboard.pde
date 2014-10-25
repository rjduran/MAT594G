// Quick File Save Function
void keyPressed() {  
  if (key == 's') {
    saveFrame(getClass().getName() + "_n" + n + "_c"+c + "-####.png");
    println("File Saved");
  }
}
