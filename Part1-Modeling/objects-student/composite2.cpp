// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Edited by Evan
//
// composite2.cpp
//
// This file defines the class for composite2 objects



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "composite2.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------
// instantiates the component parts, set their colour and relative locations,
// and establishe appropriate parentage

composite2::composite2()
{
	
	setName("composite2");

	
	body = new extrusion("bodyprofile.txt", "bodypath.txt");	
	body->useSmoothShading();
	body->setRotation('x', 90.0);
	body->attachToParentAt(this, 0, 0, 0);

	arm_pivot1 = new sphere(6);
	arm_pivot1->setScale(0.2);
	arm_pivot1->setRotation('x', 20);
	arm_pivot1->useSmoothShading();
	arm_pivot1->attachToParentAt(body, -1.1, 0, 1.7);

	arm1 = new extrusion("armprofile.txt", "armpath.txt");	
	arm1->setRotation('y', -110.0, 'x', 20);
	arm1->useSmoothShading();
	arm1->attachToParentAt(arm_pivot1, -4, 0.5, -1.4);

	arm_pivot2 = new sphere(6);
	arm_pivot2->setScale(0.2);
	arm_pivot2->setRotation('z', 180, 'x', -50);
	arm_pivot2->useSmoothShading();
	arm_pivot2->attachToParentAt(body, 1.1, 0, 1.7);

	arm2 = new extrusion("armprofile.txt", "armpath.txt");	
	arm2->setRotation('y', -110.0, 'x', 20);
	arm2->useSmoothShading();
	arm2->attachToParentAt(arm_pivot2, -4, 0.5, -1.4);

	hoof1 = new extrusion("hoofprofile.txt", "hoofpath.txt");
	hoof1->useSmoothShading();
	hoof1->setRotation('y', -90.0, 'x', -28, 'z', 90);
	hoof1->setScale(5);
	hoof1->attachToParentAt(arm1, 6.0, 0, -0.72);

	hoof2 = new extrusion("hoofprofile.txt", "hoofpath.txt");
	hoof2->useSmoothShading();
	hoof2->setRotation('y', -90.0, 'x', -28, 'z', 90);
	hoof2->setScale(5);
	hoof2->attachToParentAt(arm2, 6.0, 0, -0.72);

	pants = new extrusion("bodyprofile.txt", "pantspath.txt");
	pants->useSmoothShading();
	pants->attachToParentAt(body, 0, 0, 0);

	medal = new extrusion("bodyprofile.txt", "medalpath.txt");
	medal->useSmoothShading();
	medal->setDeformation(0.12, 0.05, 0.2);
	medal->setColour(0.988, 0.9804, 0.506);
	medal->setRotation('y',-45);
	medal->attachToParentAt(body, 0, 0.98, 2.8);

	tail_pivot = new sphere(3);
	tail_pivot->setDeformation(0.1, 0.1, 0.1);
	tail_pivot->useSmoothShading();
	tail_pivot->attachToParentAt(body, 0, -0.8, 2.8);

	tail = new extrusion("armprofile.txt", "tailpath.txt");
	tail->useSmoothShading();
	tail->setDeformation(0.05, 0.05, 0.08);
	tail->attachToParentAt(tail_pivot, 0, -0.1, 0);

	leg_pivot1 = new sphere(3);
	leg_pivot1->setDeformation(0.1, 0.1, 0.1);
	leg_pivot1->useSmoothShading();
	leg_pivot1->attachToParentAt(pants, -0.6, 0, 3.1);

	pants1 = new sweep("onepantsprofile.txt", 10);
	pants1->setRotation('x', -90.0);
	pants1->setScale(0.7);
	pants1->useSmoothShading();
	pants1->attachToParentAt(leg_pivot1, 0, 0, 1);

	leg_pivot2 = new sphere(3);
	leg_pivot2->setDeformation(0.1, 0.1, 0.1);
	leg_pivot2->useSmoothShading();
	leg_pivot2->attachToParentAt(pants, 0.6, 0, 3.1);

	pants2 = new sweep("onepantsprofile.txt", 10);
	pants2->setRotation('x', -90.0);
	pants2->setScale(0.7);
	pants2->useSmoothShading();
	pants2->attachToParentAt(leg_pivot2, 0, 0, 1);

	hoof3 = new extrusion("hoofprofile.txt", "hoofpath.txt");
	hoof3->setScale(1.2);
	hoof3->useSmoothShading();
	hoof3->attachToParentAt(pants1, 0, -0.3, 0);

	hoof4 = new extrusion("hoofprofile.txt", "hoofpath.txt");
	hoof4->setScale(1.2);
	hoof4->useSmoothShading();
	hoof4->attachToParentAt(pants2, 0, -0.3, 0);

	face = new extrusion("faceprofile.txt", "facepath.txt");
	face->useSmoothShading();
	face->setRotation('x', -10.0);
	face->attachToParentAt(body, 0, -0.2, 0);

	ear1 = new extrusion("earprofile.txt", "earpath.txt");
	ear1->useSmoothShading();	
	ear1->setRotation('x', 90, 'z', -90);
	ear1->attachToParentAt(face, 1.7, 0.05, -1.3);

	ear2 = new extrusion("earprofile.txt", "earpath.txt");
	ear2->useSmoothShading();
	ear2->setRotation('x', 90, 'z', 90);
	ear2->attachToParentAt(face, -1.7, 0.05, -1.3);

	horn1 = new sphere(10);
	horn1->setDeformation(0.12, 0.12, 0.25);
	horn1->setColour(0.1, 0.1, 0.1);
	horn1->setRotation('z', 110, 'x', 65);
	horn1->attachToParentAt(face, -0.5, 0.5, -1.3);

	horn2 = new sphere(10);
	horn2->setDeformation(0.12, 0.12, 0.25);
	horn2->setColour(0.1, 0.1, 0.1);
	horn2->setRotation('z', 110, 'x', 65);
	horn2->attachToParentAt(face, 0.5, 0.5, -1.3);

	brow1 = new	extrusion("bodyprofile.txt", "browpath.txt");
	brow1->setColour(0.1, 0.1, 0.1);
	brow1->setRotation('z', 110, 'x', 65);
	brow1->attachToParentAt(face, -1.95, 1.4, 0.15);

	brow2 = new	extrusion("bodyprofile.txt", "browpath.txt");
	brow2->setColour(0.1, 0.1, 0.1);
	brow2->setRotation('z', 100, 'x', 68);
	brow2->attachToParentAt(face, -0.75, 1.4, 0.35);

	orbit1 = new sphere(10);
	orbit1->setColour(0.1, 0.1, 0.1);
	orbit1->setRotation('z', 40, 'x', 75);
	orbit1->setDeformation(0.51, 0.55, 0.01);
	orbit1->attachToParentAt(face, -0.6, 1.13, -0.2);

	orbit2 = new sphere(10);
	orbit2->setColour(0.1, 0.1, 0.1);
	orbit2->setRotation('z', 140, 'x', 75);
	orbit2->setDeformation(0.51, 0.55, 0.01);
	orbit2->attachToParentAt(face, 0.6, 1.13, -0.2);

	spot1 = new sphere(8);
	spot1->setColour(0, 0, 0);
	spot1->setDeformation(0.07, 0.055, 0.01);
	spot1->attachToParentAt(orbit1, 0, 0.7, 0.2);

	spot2 = new sphere(8);
	spot2->setColour(0, 0, 0);
	spot2->setDeformation(0.07, 0.05, 0.01);
	spot2->attachToParentAt(orbit1, 0.4, 0.6, 0.10);

	spot3 = new sphere(8);
	spot3->setColour(0, 0, 0);
	spot3->setDeformation(0.08, 0.06, 0.01);
	spot3->attachToParentAt(orbit1, 0.55, 0.5, 0.13);

	spot4 = new sphere(8);
	spot4->setColour(0, 0, 0);
	spot4->setDeformation(0.07, 0.055, 0.01);
	spot4->attachToParentAt(orbit2, 0, -0.7, 0.22);

	spot5 = new sphere(8);
	spot5->setColour(0, 0, 0);
	spot5->setDeformation(0.07, 0.057, 0.01);
	spot5->attachToParentAt(orbit2, 0.4, -0.65, 0.16);

	spot6 = new sphere(8);
	spot6->setColour(0, 0, 0);
	spot6->setDeformation(0.06, 0.06, 0.01);
	spot6->attachToParentAt(orbit2, 0.65, -0.4, 0.1);

	eye1 = new sphere(10);
	eye1->setColour(1, 1, 1);
	eye1->setDeformation(0.4335, 0.4675, 0.01);
	eye1->attachToParentAt(orbit1, 0, 0, -0.01);

	eye2 = new sphere(10);
	eye2->setColour(1,1,1);
	eye2->setDeformation(0.4335,0.4675,0.01);
	eye2->attachToParentAt(orbit2, 0, 0, -0.01);

	ball1 = new	sphere(10);
	ball1->setColour(0, 0, 0);
	ball1->setDeformation(0.23, 0.234, 0.01);
	ball1->attachToParentAt(eye1, 0, 0, -0.05);

	ball2 = new	sphere(10);
	ball2->setColour(0, 0, 0);
	ball2->setDeformation(0.23, 0.234, 0.01);
	ball2->attachToParentAt(eye2, 0, 0, -0.05);

	nose1 = new torus(8, 10, 0.63, 0.15);
	nose1->setRotation('x', 30.0, 'z', -30);
	nose1->attachToParentAt(face, 0.54, 1.05, 1.05);
	nose1->setScale(0.2);


	nose2 = new torus(8, 10, 0.63, 0.15);
	nose2->setRotation('x', 30.0, 'z', 30);
	nose2->attachToParentAt(face, -0.54, 1.05, 1.05);
	nose2->setScale(0.2);


	nose_ring = new torus(16, 16, 1.4, 0.22);
	nose_ring->setColour(0.6, 0.6, 0.6);
	nose_ring->setRotation('x', -45);
	nose_ring->attachToParentAt(face, 0, 1.1, 1.05);
	nose_ring->setScale(0.40);

	sword = new extrusion("quadprofile.txt", "swordpath.txt");
	sword->useSmoothShading();
	sword->setColour(0.88, 0.88, 0.95);
	sword->setRotation('z', 30);
	sword->setScale(0.8);
	sword->attachToParentAt(hoof1, 0, -0.1, -1.5);

	handle = new sweep("handleprofile.txt", 8);
	handle->useSmoothShading();
	handle->setRotation('x', 90);
	handle->setColour(0.5, 0.25, 0.08);
	handle->setDeformation(0.4, 0.6, 0.4);
	handle->attachToParentAt(sword, 0, 0, 1.85);

	appendant = new torus(8, 10, 0.25, 0.2);
	appendant->useSmoothShading();
	appendant->setRotation('x', 90);
	appendant->setDeformation(1, 0.15, 0.5);
	appendant->setColour(0.73, 0.91, 0.98);	
	appendant->attachToParentAt(sword, 0, 0, 1.5);


	shield = new sweep("shieldprofile.txt", 16);
	shield->setRotation('z', 180);
	shield->setColour(0.906, 0.771, 0.616);
	shield->setScale(1.5);
	shield->attachToParentAt(hoof2, 0, 0, 0);

	sh_ring = new torus(10, 16, 0.6, 0.06);
	sh_ring->useSmoothShading();
	sh_ring->setColour(0.83, 0.91, 0.98);
	sh_ring->attachToParentAt(shield, 0, 0.085, 0);

	sh_handle1 = new sweep("handleprofile.txt", 8);
	sh_handle1->useSmoothShading();
	sh_handle1->setDeformation(0.1, 0.15, 0.1);
	sh_handle1->attachToParentAt(shield, 0, 0.1, 0.15);

	sh_handle2 = new sweep("handleprofile.txt", 8);
	sh_handle2->useSmoothShading();
	sh_handle2->setDeformation(0.1, 0.15, 0.1);
	sh_handle2->attachToParentAt(shield, 0, 0.1, -0.15);

	sh_handle3 = new sweep("handleprofile.txt", 8);
	sh_handle3->useSmoothShading();
	sh_handle3->setRotation('x', 90);
	sh_handle3->setDeformation(0.1, 0.27, 0.1);
	sh_handle3->attachToParentAt(shield, 0, 0.01, 0);

	

	// set the default colors
	setBodycolor(255, 233, 45);
	setFacecolor(255, 233, 45);
	setFootcolor(138, 94, 45);
	setPantscolor(0, 0, 179);
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}



// -----------------------------------------------------------------------------------------
// set the color of each part 
// -----------------------------------------------------------------------------------------
// these methods change the RGB range from 0-255 to 0.0-1.0
// -----------------------------------------------------------------------------------------

void composite2::setBodycolor(float r, float g, float b)
{
	float R = r/255.0;
	float G = g/255.0;
	float B = b/255.0;

	body->setColour(R, G, B);
	arm_pivot1->setColour(R, G, B);
	arm_pivot2->setColour(R, G, B);
	arm1->setColour(R, G, B);
	arm2->setColour(R, G, B);
	tail->setColour(R, G, B);
}

void composite2::setFacecolor(float r, float g, float b)
{
	float R = r/255.0;
	float G = g/255.0;
	float B = b/255.0;

	face->setColour(R, G, B);
	ear1->setColour(R, G, B);
	ear2->setColour(R, G, B);
	nose1->setColour(R/1.5,G/1.5,B/1.5);
	nose2->setColour(R/1.5,G/1.5,B/1.5);
}


void composite2::setFootcolor(float r, float g, float b)
{
	float R = r/255.0;
	float G = g/255.0;
	float B = b/255.0;

	hoof1->setColour(R, G, B);
	hoof2->setColour(R, G, B);
	hoof3->setColour(R, G, B);
	hoof4->setColour(R, G, B);
}

void composite2::setPantscolor(float r, float g, float b)
{
	float R = r/255.0;
	float G = g/255.0;
	float B = b/255.0;

	pants->setColour(R, G, B);
	pants1->setColour(R, G, B);
	pants2->setColour(R, G, B);
}


// -----------------------------------------------------------------------------------------
// hide & show
// -----------------------------------------------------------------------------------------
// these methods override those of object3d, and cascade the hide and show message to
// all of the component parts
// -----------------------------------------------------------------------------------------
void composite2::hide()
{
	hide_equip();

	face->hide();
	body->hide();
	pants->hide();
	tail->hide();
	medal->hide();

	arm1->hide();
	arm2->hide();

	ear1->hide();
	ear2->hide();

	brow1->hide();
	brow2->hide();

	hoof1->hide();
	hoof2->hide();
	hoof3->hide();
	hoof4->hide();

	pants1->hide();
	pants2->hide();
		
	arm_pivot1->hide();
	arm_pivot2->hide();
	leg_pivot1->hide();
	leg_pivot2->hide();
	tail_pivot->hide();

	spot1->hide();
	spot2->hide();
	spot3->hide();
	spot4->hide();
	spot5->hide();
	spot6->hide();

	eye1->hide();
	orbit1->hide();
	ball1->hide();

	eye2->hide();
	orbit2->hide();
	ball2->hide();

	horn1->hide();
	horn2->hide();

	nose1->hide();
	nose2->hide();
	nose_ring->hide();
}

void composite2::show()
{
	show_equip();

	face->show();
	body->show();
	pants->show();
	tail->show();
	medal->show();

	arm1->show();
	arm2->show();

	ear1->show();
	ear2->show();

	brow1->show();
	brow2->show();

	hoof1->show();
	hoof2->show();
	hoof3->show();
	hoof4->show();

	pants1->show();
	pants2->show();
		
	arm_pivot1->show();
	arm_pivot2->show();
	leg_pivot1->show();
	leg_pivot2->show();
	tail_pivot->show();

	spot1->show();
	spot2->show();
	spot3->show();
	spot4->show();
	spot5->show();
	spot6->show();

	eye1->show();
	orbit1->show();
	ball1->show();

	eye2->show();
	orbit2->show();
	ball2->show();

	horn1->show();
	horn2->show();

	nose1->show();
	nose2->show();
	nose_ring->show();
}


// -----------------------------------------------------------------------------------------
// set the visible of decorations
// -----------------------------------------------------------------------------------------
// these methods decide whether the equipment of character will be show
// -----------------------------------------------------------------------------------------
void composite2::hide_equip()
{
	hide_shield();
	hide_sword();
}

void composite2::show_equip()
{
	sword->show();
	handle->show();
	appendant->show();
	sh_handle1->show();
	sh_handle2->show();
	sh_handle3->show();
	shield->show();
	sh_ring->show();
}

void composite2::show_sword_only()
{
	//just show the sword on the center
	//sword->setPosition(0,0,0);
	
	sword->show();
	handle->show();
	appendant->show();

	hide_shield();
}

void composite2::hide_sword()
{
	sword->hide();
	handle->hide();
	appendant->hide();
}


void composite2::show_shield_only()
{
	//just show the shield on the center
	//shield->setPosition(0,0,0);

	sh_handle1->show();
	sh_handle2->show();
	sh_handle3->show();
	shield->show();
	sh_ring->show();

	hide_sword();
}

void composite2::hide_shield()
{
	sh_handle1->hide();
	sh_handle2->hide();
	sh_handle3->hide();
	shield->hide();
	sh_ring->hide();
}