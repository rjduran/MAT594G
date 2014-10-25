/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

// scene constants
const float wallMass = 0.1;
const int maxIterations = 40;

// constant cannon settings
const float cannonX = -10.0;
const float cannonY = 0.0;
const float cannonBallMass = 10.0;
const float cannonBallRadius = 1;

// ODE objects
dWorldID world;
dSpaceID space;

dJointGroupID contactgroup; // joint group for boxes
vector<dGeomID> wallBoxes;
vector<dBodyID> wallBodies; // a vector of a bunch of dBodyID objects

dBodyID cannonBallBody;
dGeomID cannonBallGeom;

// user-definable cannon settings
dReal turn = 0;
dReal cannonAngle = 0;
dReal cannonElevation = -1.2;

vector<dGeomID> chainSpheres;
vector<dBodyID> chainBodies;

struct burstbox {
  dGeomID id;
  bool burstable;
  float mag;
} bb[210], bs[11];


///////////////////////////////////////////////////////////////////////
// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
///////////////////////////////////////////////////////////////////////
void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  // get the ids of each body
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);

  if (b1 && b2 && dAreConnected(b1, b2))
    return;
  
  const int N = 4;
  dContact contact[N]; // array of 4 contact joints
  int n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));  // generate number of contact points between two geoms

  if (n > 0) 
  {
    // loop through all the contact points created by collision
    for (int i = 0; i < n; i++) 
    {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;

      if (dGeomGetClass(o1) == dSphereClass || dGeomGetClass(o2) == dSphereClass) {
        contact[i].surface.mu = 20;        
      } else {
        contact[i].surface.mu = 0.5;
      }

      contact[i].surface.slip1 = 0.0;
      contact[i].surface.slip2 = 0.0;
      contact[i].surface.soft_erp = 0.8;
      contact[i].surface.soft_cfm = 0.01;

      dJointID c = dJointCreateContact(world,contactgroup,contact+i);
      dJointAttach(c,dGeomGetBody(o1),dGeomGetBody(o2));

      // if the joint c created is between the canonball and chain
      // break link

      if(b1 == cannonBallBody && b2 == chainBodies[2] && bs[2].burstable) {        
        dJointID joint0 = dBodyGetJoint(chainBodies[2], 0);
        dJointID joint1 = dBodyGetJoint(chainBodies[1], 0);
      
        dJointDestroy(joint0);
        dJointDestroy(joint1);
        bs[2].burstable = false;                
        return;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////
// Initialize the rigid bodies in the scene
///////////////////////////////////////////////////////////////////////
void initSimulation()
{
  // create world
  world = dWorldCreate();
  space = dSweepAndPruneSpaceCreate(0, dSAP_AXES_XYZ);

  // joint group
  contactgroup = dJointGroupCreate(0);
  dWorldSetGravity(world,0,0,-1.5);
  dWorldSetCFM(world, 1e-5);
  dWorldSetERP(world, 0.8);
  dWorldSetQuickStepNumIterations(world, maxIterations);
  
  // define ground plane
  dGeomID ground = dCreatePlane(space,0,0,1,0); // Z normal UP
  
  // create the wall  
  const float boxSize = 1;
  const int wallWidth = 20;   
  const float wallHeight = 10;
  dMass mass;
	for (dReal z = boxSize/2.0; z <= wallHeight; z+=boxSize)
  {		
		for (dReal y = (-wallWidth)/2; y <= wallWidth/2; y+=boxSize)
    {
      dBodyID wallBody = dBodyCreate(world); // create a body in the world
			dBodySetPosition (wallBody,-24,y,z); // set position of wallBody
			dMassSetBox (&mass,1,boxSize,boxSize,boxSize); // set its dimensions
			dMassAdjust (&mass, wallMass);   
			dBodySetMass (wallBody,&mass);
      dGeomID newBox = dCreateBox(space, boxSize, boxSize,boxSize); // create a new geom in space

      wallBoxes.push_back(newBox); // put new geometry id into vector
      wallBodies.push_back(wallBody); // put new body id into vector
      dGeomSetBody(wallBoxes.back(), wallBody); // set the body for last geom in wallBoxes vector
		}
	}
  
  // create a cannon
  cannonBallBody = dBodyCreate(world);
  cannonBallGeom = dCreateSphere(space, cannonBallRadius);
  dMassSetSphereTotal(&mass, cannonBallMass, cannonBallRadius);
  dBodySetMass(cannonBallBody, &mass);
  dGeomSetBody(cannonBallGeom, cannonBallBody);
  dBodySetPosition(cannonBallBody, cannonX, cannonY, cannonBallRadius);


  // make array of burstbox and associate the id and init burstable to false
  //burstbox b[4];
  // b[0].id = wallBoxes[0];
  // b[0].burstable = false;

  // b[1].id = wallBoxes[1];
  // b[1].burstable = true;

  for (int i = 0; i < wallBoxes.size(); ++i)
  {
    bb[i].id = wallBoxes[i];
    bb[i].burstable = false;
    bb[i].mag = 0.0;

    if (i > 62 && i < wallBoxes.size() - 63) {
      bb[i].burstable = true;
    } 
  
    //cout << i << " : " << bb[i].id << " : " << bb[i].burstable << endl;
  }

  // dVector3 v0;
  // v0[0] = 0.1;
  // v0[1] = 0.2;
  // v0[2] = 0.3;

  // cout << v0[0] << endl;
  // float mag[wallBodies.size()];
  // for (int i = 0; i < wallBodies.size(); i++) {    
  //   mag[i]  = 0.0;
  // }
  
  
  // create chain links
  float linkSize = 0.5;  // size of a link in the chain
  const int chainLength = 11; // length of chain
  dMass m;
  dReal k;

  for (int i = 0; i < chainLength; i++) {
    dBodyID chainBody = dBodyCreate(world);            // make body
    k = 15 - i; 

    if( i == 10 ) {
      k = 15 - i - 1.5; 
      linkSize = 2.0;
    } 
    
    dBodySetPosition(chainBody, -20, 0, k);            // set position of top link
    dMassSetBox(&m, 1, linkSize, linkSize, linkSize); // set mass?
    dMassAdjust(&m, 1.0);                             // adjust mass?
    dBodySetMass(chainBody, &m);                       // set mass of body
    dBodySetData(chainBody, (void*)(size_t)i);         // set the body's user datab pointer?
    
    dGeomID chainSphere = dCreateSphere(space, linkSize); // create a new sphere    
    chainSpheres.push_back(chainSphere);   // push the chainSphere into vector of dGeomID's
    chainBodies.push_back(chainBody);  // push the chainBody into vector of dBodyID's

    dGeomSetBody(chainSpheres.back(), chainBody); // set the body for the box

    if (i == 0) {
      dBodySetKinematic(chainBody); // set the body to kinematic aka fixed                                       
    }
  }

  for (int i = 0; i < (chainLength - 1); ++i){      
    dJointID joint = dJointCreateBall(world, 0);
    dJointAttach(joint, chainBodies[i], chainBodies[i+1]);
    
    k = 15 - (i+0.5);  
    if( i == 10 ) {
      k = 15 - (i+0.75) - 1.5;         
    }

    dJointSetBallAnchor(joint, -20, 0, k);
  }

  // set sphere 3 as burstable
  for (int i = 0; i < chainSpheres.size(); ++i)
  {
    bs[i].id = chainSpheres[i];
    bs[i].burstable = false;
    bs[i].mag = 0.0;

    if (i == 2) {
      bs[i].burstable = true;
    } 
  }  

}

///////////////////////////////////////////////////////////////////////
// main simulation loop, also does the drawing to the screen
///////////////////////////////////////////////////////////////////////
void simulationLoop(int pause)
{

  // previous timestep 
  float vx[wallBodies.size()];
  float vy[wallBodies.size()];
  float vz[wallBodies.size()];
    
  // store velocities of v(t-1)
  for (int i = 0; i < wallBodies.size(); i++) {
    vx[i] = dBodyGetLinearVel(wallBodies[i])[0];
    vy[i] = dBodyGetLinearVel(wallBodies[i])[1];
    vz[i] = dBodyGetLinearVel(wallBodies[i])[2];  
  }  

  // do narrow phase collision detection and resolution  
  dSpaceCollide(space, 0, &nearCallback);

  // step the world
  const float dt = 1.0 / 20.0; // simulation timestep delta_t
  dWorldQuickStep(world, dt);

  // delete the contacts, since dWorldQuickStep just resolved them
  dJointGroupEmpty(contactgroup);
  


  // draw wrecking ball   
  for (int i = 0; i < chainSpheres.size(); i++) 
  {  
    if( bs[i].burstable ) {
      dsSetColor(1,0,0); // red
    } else {
      dsSetColor(1,1,1); // white
    }

        
    dReal r = dGeomSphereGetRadius(chainSpheres[i]);  // get radius of sphere
    dsDrawSphere(dGeomGetPosition(chainSpheres[i]), dGeomGetRotation(chainSpheres[i]), r);    
  }
  

  // draw the dynamic boxes 
  for (int i = 0; i < wallBoxes.size(); i++)  // for all the geoms
  {    
    // color boxes    
    if( bb[i].burstable ) {
      dsSetColor(1,0,0); // red
    } else {
      dsSetColor(1,1,1); // white
    }  

    if ( bb[i].mag > 50 && bb[i].burstable ) {
      dGeomSetPosition(bb[i].id,-20,0,-9999); // set position of object to far away
      bb[i].burstable = false; // once moved, make non-burstable
    }
    
    dVector3 ss;
    dGeomBoxGetLengths(wallBoxes[i], ss);
    dsDrawBox(dGeomGetPosition(wallBoxes[i]), dGeomGetRotation(wallBoxes[i]), ss);
    
  }

  // draw the cannon
  dMatrix3 R2,R3,R4;
  dRFromAxisAndAngle(R2, 0, 0, 1, cannonAngle);
  dRFromAxisAndAngle(R3, 0, 1, 0, cannonElevation);
  dMultiply0(R4, R2, R3, 3, 3, 3);
  dReal cpos[3] = {cannonX, cannonY, 1};
  dReal csides[3] = {2, 2, 2};
  dsDrawBox(cpos, R2, csides);

  for (int i = 0; i < 3; i++) 
    cpos[i] += 1.5 * R4[i * 4 + 2];

  // draw the cannon barrel
  dsDrawCylinder(cpos, R4, 3, 0.5);
 
  // draw the cannonball
  dsDrawSphere(dBodyGetPosition(cannonBallBody),
               dBodyGetRotation(cannonBallBody),
               cannonBallRadius);
  
  // calculate acceleration
  for (int i = 0; i < wallBodies.size(); i++) {  

    float vx1 = dBodyGetLinearVel(wallBodies[i])[0];
    float vy1 = dBodyGetLinearVel(wallBodies[i])[1];
    float vz1 = dBodyGetLinearVel(wallBodies[i])[2];    

    float accelX = (vx1 - vx[i]) / dt;
    float accelY = (vy1 - vy[i]) / dt;
    float accelZ = (vy1 - vy[i]) / dt;

    bb[i].mag = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);   
  }   
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void keyboard(int key)
{
  switch (key)
  {
    case ' ':
      {
        dMatrix3 R2,R3,R4;
        dRFromAxisAndAngle(R2,0,0,1,cannonAngle);
        dRFromAxisAndAngle(R3,0,1,0,cannonElevation);
        dMultiply0(R4,R2,R3,3,3,3);
        dReal cpos[3] = {cannonX,cannonY,1};
        for (int i=0; i<3; i++) cpos[i] += 3*R4[i*4+2];
        dBodySetPosition (cannonBallBody,cpos[0],cpos[1],cpos[2]);
        dReal force = 10;
        dBodySetLinearVel (cannonBallBody,force*R4[2],force*R4[6],force*R4[10]);
        dBodySetAngularVel (cannonBallBody,0,0,0);
      }
      break;
    case 'j':
      cannonAngle += 0.1;
      break;
    case 'l':
      cannonAngle -= 0.1;
      break;
    case 'i':
      cannonElevation += 0.1;
      break;
    case 'k':
      cannonElevation -= 0.1;
      break;
    case 'q':
      exit(0);
      break;
    default:
      break;
  }
}

///////////////////////////////////////////////////////////////////////
// Set up the initial viewing point
///////////////////////////////////////////////////////////////////////
void start()
{
	float xyz[3] = {0,0,5};
	float hpr[3] = {180,0,0};
	dsSetViewpoint (xyz,hpr);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
	// setup pointers to drawstuff callback functions
	dsFunctions function;
	function.start = &start;
	function.step = &simulationLoop;
	function.command = &keyboard;
	
	dInitODE2(0);
	initSimulation();

	// run simulation
	dsSimulationLoop (argc,argv,800,800,&function);
	
	dJointGroupDestroy (contactgroup);
	dSpaceDestroy (space);
	dWorldDestroy (world);
	dCloseODE();
	return 0;
}
