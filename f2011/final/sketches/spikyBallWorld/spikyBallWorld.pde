
import wblut.processing.*;
import wblut.hemesh.creators.*;
import wblut.hemesh.core.*;
import wblut.hemesh.modifiers.*;
import wblut.geom.*;

//HE_Mesh mesh;
WB_Render render;
HEM_Extrude modifier;
//HEM_Twist twistModifier;
//WB_Line L;

float[][] points;
int numpoints;
//HE_Mesh container;
HE_Mesh[] cells;
int numcells;
HEMC_VoronoiSpheres multiCreator;

void setup() {
  size(800, 800, P3D);

  // create mesh
  numpoints=50;
  points=new float[numpoints][3];
  for (int i=0;i<numpoints;i++) {
    points[i][0]=random(-250, 250);
    points[i][1]=random(-250, 250);
    points[i][2]=random(-250, 250);
  }

  createMesh();

  // extrude modifier
  modifier=new HEM_Extrude();
  modifier.setDistance(60);// extrusion distance, set to 0 for inset faces
  modifier.setRelative(false);// treat chamfer as relative to face size or as absolute value
  modifier.setChamfer(10);// chamfer for non-hard edges
  modifier.setHardEdgeChamfer(10);// chamfer for hard edges
  modifier.setThresholdAngle(1.5*HALF_PI);// treat edges sharper than this angle as hard edges
  modifier.setFuse(false);// try to fuse planar adjacent planar faces created by the extrude
  modifier.setFuseAngle(0.05*HALF_PI);// threshold angle to be considered coplanar
  modifier.setPeak(true);//if absolute chamfer is too large for face, create a peak on the face

    for (int i=0; i< numcells; i++) {
    cells[i].modify(modifier);
  }


  // twist modifier
  //  twistModifier=new HEM_Twist();
  //  L=new WB_Line(100, 0, 0, 50, 0, 1);
  //
  //  twistModifier.setTwistAxis(L);// Twist axis
  //  twistModifier.setAngleFactor(.5);
  //  mesh.modify(twistModifier);

  // render
  render=new WB_Render(this);
}

void draw() {
  background(255);
  directionalLight(255, 255, 255, 1, 1, -1);
  directionalLight(127, 127, 127, -1, -1, 1);
  translate(400, 400, 100);
  //rotateY(mouseX*1.0f/width*TWO_PI);
  //rotateX(mouseY*1.0f/height*TWO_PI);
  //  stroke(0);
  //  render.drawEdges(mesh);
  //  noStroke();
  //  render.drawFaces(mesh);
  drawFaces();
  drawEdges();


  // re-create mesh
  createMesh();

  modifier.setDistance(map(mouseX, 0, width, -100, 200));// extrusion distance, set to 0 for inset faces
  //modifier.setChamfer(map(mouseY, 0, height, 1, 20));// chamfer for non-hard edges

  // add modifications
  for (int i=0; i< numcells; i++) {
    cells[i].modify(modifier);
  }
  //
  //  L=new WB_Line(-200+mouseX*0.5,0,0,-200+mouseX*0.5,0,10);
  //  twistModifier.setTwistAxis(L);
  //  twistModifier.setAngleFactor(mouseY*0.005);
  //  mesh.modify(twistModifier);
}

void createMesh() {
  // generate voronoi cells
  multiCreator=new HEMC_VoronoiSpheres();
  multiCreator.setPoints(points);
  // alternatively points can be WB_Point[], any Collection<WB_Point> and double[][];
  multiCreator.setN(50);//number of points, can be smaller than number of points in input. 
  multiCreator.setLevel(2);// subdivision level for cell spheres
  multiCreator.setCutoff(20);// maximum radius of cell
  multiCreator.setApprox(true);// approximate cells by point expansion or precise cells by sphere slicing
  multiCreator.setNumTracers(100);// random points per cell in approcimate mode
  multiCreator.setTraceStep(1);// step size for random points expansion
  cells=multiCreator.create();
  numcells=cells.length;
}

void drawEdges() {
  smooth();
  stroke(0);
  for (int i=0;i<numcells;i++) {
    render.drawEdges(cells[i]);
  }
}

void drawFaces() {
  noSmooth();
  noStroke();
  fill(255);
  for (int i=0;i<numcells;i++) {
    render.drawFaces(cells[i]);
  }
}

