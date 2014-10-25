import wblut.hemesh.modifiers.*;
import wblut.processing.*;
import wblut.hemesh.creators.*;
import wblut.hemesh.core.*;

HE_Mesh mesh;
WB_Render render;
HEM_Noise modifier;

float x,y;

void setup() {
  size(800, 800, P3D);
  createMesh();
  
  modifier=new HEM_Noise();
  modifier.setDistance(10);
  mesh.modify(modifier);

  render=new WB_Render(this);
  
  x = 0.0;
  y = 0.0;
}

void draw() {
  background(120);
  directionalLight(255, 255, 255, 1, 1, -1);
  directionalLight(127, 127, 127, -1, -1, 1);
  translate(400,400, 0);
  rotateY(y*1.0f/width*TWO_PI);
  rotateX(x*1.0f/height*TWO_PI);
  fill(255);
  noStroke();
  
  modifier.setDistance(random(0, 2));
  mesh.modify(modifier);
  
  render.drawFaces(mesh);
  stroke(0);
  render.drawEdges(mesh);
  x+=0.9;
  y+=0.3;
}


void createMesh(){
  HEC_Cube creator=new HEC_Cube(300,5,5,5);
  mesh=new HE_Mesh(creator); 
}

