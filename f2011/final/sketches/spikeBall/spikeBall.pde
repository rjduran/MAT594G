
import wblut.processing.*;
import wblut.hemesh.creators.*;
import wblut.hemesh.core.*;
import wblut.hemesh.modifiers.*;

HE_Mesh mesh;
WB_Render render;
HEM_Extrude modifier;

void setup() {
  size(800, 800, P3D);

  // create mesh
  createMesh();

  // modify
  modifier=new HEM_Extrude();
  modifier.setDistance(30);// extrusion distance, set to 0 for inset faces
  modifier.setRelative(false);// treat chamfer as relative to face size or as absolute value
  modifier.setChamfer(10);// chamfer for non-hard edges
  modifier.setHardEdgeChamfer(10);// chamfer for hard edges
  modifier.setThresholdAngle(1.5*HALF_PI);// treat edges sharper than this angle as hard edges
  modifier.setFuse(false);// try to fuse planar adjacent planar faces created by the extrude
  modifier.setFuseAngle(0.05*HALF_PI);// threshold angle to be considered coplanar
  modifier.setPeak(true);//if absolute chamfer is too large for face, create a peak on the face
  mesh.modify(modifier);

  render=new WB_Render(this);
}

void draw() {
  background(255);
  directionalLight(255, 255, 255, 1, 1, -1);
  directionalLight(127, 127, 127, -1, -1, 1);
  translate(400, 400, 100);
  rotateY(mouseX*1.0f/width*TWO_PI);
  rotateX(mouseY*1.0f/height*TWO_PI);
  stroke(0);
  render.drawEdges(mesh);
  noStroke();
  render.drawFaces(mesh);

  // re-create mesh
  createMesh();

  modifier.setDistance(map(mouseX, 0, width, -100, 100));// extrusion distance, set to 0 for inset faces
  modifier.setChamfer(map(mouseY, 0, height, 1, 20));// chamfer for non-hard edges

  //add modifications
  mesh.modify(modifier);
}

void createMesh() {
  HEC_Sphere creator=new HEC_Sphere();
  creator.setRadius(200); 
  creator.setUFacets(16);
  creator.setVFacets(16);
  mesh=new HE_Mesh(creator);
}

