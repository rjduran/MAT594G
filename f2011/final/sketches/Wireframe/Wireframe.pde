import processing.opengl.*;

import wblut.hemesh.modifiers.*;
import wblut.processing.*;
import wblut.hemesh.creators.*;
import wblut.hemesh.core.*;

HE_Mesh mesh;
WB_Render render;

void setup() {
  size(1000, 1000, OPENGL);
  createMesh();

  HEM_Wireframe modifier=new HEM_Wireframe();
  modifier.setStrutRadius(10);// strut radius
  modifier.setStrutFacets(4);// number of faces in the struts, min 3, max whatever blows up the CPU
 // modifier.setMaximumStrutOffset(20);// limit the joint radius by decreasing the strut radius where necessary. Joint offset is added after this limitation.
  modifier.setAngleOffset(1);// rotate the struts by a fraction of a facet. 0 is no rotation, 1 is a rotation over a full facet. More noticeable for low number of facets.
  modifier.setTaper(false);// allow struts to have different radii at each end?
  mesh.modify(modifier);
//mesh.modify(new HEM_Slice().setPlane(0,0,-50,0,0,1));

  render=new WB_Render(this);
}

void draw() {
  background(120);
  directionalLight(255, 255, 255, 1, 1, -1);
  directionalLight(127, 127, 127, -1, -1, 1);
  translate(500, 500, 100);
  rotateY(mouseX*1.0f/width*TWO_PI);
  rotateX(mouseY*1.0f/height*TWO_PI);
  fill(255);
  noStroke();
  render.drawFaces(mesh);
  stroke(0);
  render.drawEdges(mesh);
}

void createMesh(){
  HEC_SuperDuper sd=new HEC_SuperDuper();
  sd.setU(3).setV(3).setRadius(80); // setU is the number of outside vertices, setV is the inside shape sides at each vertex 
  sd.setDonutParameters(0, 1, 1, 1,    3, 1, 1, 1,    3,3);
  mesh=new HE_Mesh(sd); 
}

/*
process

SuperDuper object made
added to a new Mesh
Wireframe modifier is made
modifier params set
mesh.modify adds the modifier to mesh?
render



setDonutParameters() params
double m1,
double n11,
double n12,
double n13,
double m2,
double n21,
double n22,
double n23,
double t, // changes how much the shape opens up?
double c)
*/

