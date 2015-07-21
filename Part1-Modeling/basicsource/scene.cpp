// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// scene.cpp
//
// Modified by Evan



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "constants.h"			// system-wide constants
#include "utility.h"			// general utility functions
#include "light.h"
#include "camera.h"
#include "texture.h"
#include "scene.h"
#include "snd.h"
#include "skybox.h"
#include "sweep.h"
#include "extrusion.h"

#include "opencube.h"
#include "opensphere.h"
#include "opentorus.h"
#include "composite1.h"
#include "composite2.h"


// -----------------------------------------------------------------------------------------
// global variables
// -----------------------------------------------------------------------------------------
extern ProgramMode		gProgramMode;							// initially, we're not animating

extern cameraClass		gCamera;								// the camera
extern object3d			*gSky;									// the global skybox
extern float			gCurrentFrameTime;						// the time that the current animation frame was started

extern vector<object3d*> gShapeVector;
typedef vector<object3d*>::iterator shapeVectorIterator;






// -----------------------------------------------------------------------------------------
// variables that represent 3D objects being used in the test suite
// -----------------------------------------------------------------------------------------
// these are really global variables, but you shouldn't need to access them from anywere
// except in this file 
// -----------------------------------------------------------------------------------------

// YOU SHOULD ASSIGN YOUR REQUIRED SHAPES
// TO THESE VARIABLES IN CONSTRUCTSCENE()
// PRIOR TO SUBMITTING YOUR ASSIGNMENT
object3d		*test_opencube = 0;
object3d		*test_opensphere = 0;
object3d		*test_opentorus = 0;
object3d		*test_extrusion1 = 0;
object3d		*test_extrusion2 = 0;
object3d		*test_sweep1 = 0;
object3d		*test_sweep2 = 0;
object3d		*test_composite1 = 0;
object3d		*test_composite2 = 0;


// complete shapes
opencube		*c;
opensphere		*s;
opentorus		*t;
sweep			*s1,*s2;
extrusion		*e1,*e2;
composite1		*c1;
composite2		*c2;



// some lights
light *ambient, *light0, *light1;





// -----------------------------------------------------------------------------------------
// constructScene
// -----------------------------------------------------------------------------------------
// This function constructs the objects required for the animation.  It is only called once
// when the program is first run.
// Use this to create child objects and bind them to their parents.
// -----------------------------------------------------------------------------------------

void constructScene()
{
	// initialise the camera
	gCamera.setPosition(0, 4, 12);
	gCamera.setTarget(0, 0, 0);
	
	
	// create the lights first
	ambient = new light(GL_LIGHT_MODEL_AMBIENT);
	light0 = new light(GL_LIGHT0);
	light1 = new light(GL_LIGHT1);
	
	// define the light characteristics
	ambient->setColour(0.5, 0.5, 0.5, 1.0);
	
	light0->turnOn();
	light0->setPosition(-10, 10, 5);
	light0->setColour(.5, .5, .5, 1.0);
	light0->setSpecularColour(.5, .5, .5, 1.0);
	
	light1->turnOn();
	light1->setPosition(10, 10, -5);
	light1->setColour(.5, .5, .5, 1.0);
	light1->setSpecularColour(.5, .5, .5, 1.0);	

	// create some objects of complete shapes
	c = new opencube();
	c->setColour(0, 0, 1);
	c->setPosition(-4, 0, 0);
	
	s = new opensphere(16);
	s->setColour(1, 0, 0);
	s->setPosition( 0, 0, -3);
	
	t = new opentorus(16, 24, 1.5, 0.4);
	t->setColour(0, 1, 0);
	t->setPosition(5, 0, 0);

	s1 = new sweep("shieldprofile.txt", 16);
	s1->useDiffuseShading();
	s1->setColour(0.906, 0.771, 0.616);
	s1->setRotation('x', 90);
	s1->setPosition(3, 0, 4);

	s2 = new sweep("diamondprofile.txt", 8);
	s2->useDiffuseShading();
	s2->setColour(0.73, 0.91, 0.98, 0.8);
	//s2->setEmissiveColour(0.73,0.91,0.9,0.1);
	s2->setPosition(0, -4, 0);

	e1 = new extrusion("quadprofile.txt", "swordpath.txt");
	e1->useSmoothShading();
	e1->setRotation('x', 90);	
	e1->setColour(0.9, 0.9, 0.98);
	e1->setPosition(0, 0, -5);

	e2 = new extrusion("hoofprofile.txt", "hoofpath.txt");
	e2->useSmoothShading();	
	e2->setScale(2);
	e2->setPosition(0, 2, -3);

	c1 = new composite1();
	c1->setPosition(-3, 0, 5);
	//c1->closebox(9);
	c1->openbox(3);		// show the opened box

	c2 = new composite2();
	c2->setPosition(0,2,0);
	//c2->show_sword_only();

	
	// these assignments have been changed correctly 
	test_opencube = c;
	test_opensphere = s;
	test_opentorus = t;
	test_extrusion1 = e1;
	test_extrusion2 =e2;
	test_sweep1 = s1;
	test_sweep2 = s2;
	test_composite1 = c1;
	test_composite2 = c2;

}






