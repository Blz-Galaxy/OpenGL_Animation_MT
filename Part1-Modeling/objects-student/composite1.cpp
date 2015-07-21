// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Edited by Evan
//
// composite1.cpp
//
// This file defines the class for composite1 objects



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "composite1.h"
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

composite1::composite1()
{
	
	setName("composite1");

	outface = new treasurebox();
	outface->setColour(0.706, 0.471, 0.216);
	outface->attachToParentAt(this, 0, 0, 0);

	insideface = new treasurebox();
	insideface->setColour(0.706, 0.471, 0.216);
	insideface->setScale(0.95);
	insideface->attachToParentAt(outface, 0, 0.01, 0);

	h_frame1 = new extrusion("frameprofile.txt","framepath.txt");
	h_frame1->useSmoothShading();
	h_frame1->setColour(0.980, 0.957, 0.173);
	h_frame1->attachToParentAt(outface, 0, 0.5, 0);

	h_frame2 = new extrusion("frameprofile.txt","framepath.txt");
	h_frame2->useSmoothShading();
	h_frame2->setDeformation(0.9, 1, 1);
	h_frame2->setColour(0.980, 0.957, 0.173);
	h_frame2->attachToParentAt(outface, 0, -1, 0);

	v_frame1 = new extrusion("frameprofile.txt","framepath2.txt");
	v_frame1->useSmoothShading();
	v_frame1->setDeformation(0.9, 1, 1);
	v_frame1->setColour(0.980, 0.957, 0.173);
	v_frame1->attachToParentAt(outface, -1.37, 0.05, 0.9);

	v_frame2 = new extrusion("frameprofile.txt","framepath2.txt");
	v_frame2->useSmoothShading();
	v_frame2->setDeformation(0.9, 1, 1);
	v_frame2->setColour(0.980, 0.957, 0.173);
	v_frame2->attachToParentAt(outface, -1.37, 0.05, -0.9);

	v_frame3 = new extrusion("frameprofile.txt","framepath2.txt");
	v_frame3->useSmoothShading();
	v_frame3->setDeformation(0.9, 1, 1);
	v_frame3->setRotation('y', 180);
	v_frame3->setColour(0.980, 0.957, 0.173);
	v_frame3->attachToParentAt(outface, 1.37, 0.05, 0.9);

	v_frame4 = new extrusion("frameprofile.txt","framepath2.txt");
	v_frame4->useSmoothShading();
	v_frame4->setDeformation(0.9, 1, 1);
	v_frame4->setRotation('y', 180);
	v_frame4->setColour(0.980, 0.957, 0.173);
	v_frame4->attachToParentAt(outface, 1.37, 0.05, -0.9);

	pivot = new cube();
	pivot->setDeformation(1, 0.01, 0.01);
	//pivot->setRotation('x', 90);
	pivot->attachToParentAt(outface, 0, 0.55, -1.05);

	lid = new extrusion("lidprofile.txt","lidpath.txt");
	lid->setColour(0.706, 0.471, 0.216);
	lid->setRotation('y', 90);
	lid->useSmoothShading();
	lid->attachToParentAt(pivot, 0, 1, -0.1);

	lid_frame1 = new extrusion("frameprofile.txt","framepath3.txt");
	lid_frame1->useSmoothShading();
	lid_frame1->setDeformation(0.9, 1, 1);
	lid_frame1->setRotation('y', 90, 'z', -10);
	lid_frame1->setColour(0.980, 0.957, 0.173);
	lid_frame1->attachToParentAt(lid, -0.3, 0, 1.5);

	lid_frame2 = new extrusion("frameprofile.txt","framepath3.txt");
	lid_frame2->useSmoothShading();
	lid_frame2->setDeformation(0.9, 1, 1);
	lid_frame2->setRotation('y', 90, 'z', -10);
	lid_frame2->setColour(0.980, 0.957, 0.173);
	lid_frame2->attachToParentAt(lid, -0.3, 0, 0);

	lid_frame3 = new extrusion("frameprofile.txt","framepath3.txt");
	lid_frame3->useSmoothShading();
	lid_frame3->setDeformation(0.9, 1, 1);
	lid_frame3->setRotation('y', 90, 'z', -10);
	lid_frame3->setColour(0.980, 0.957, 0.173);
	lid_frame3->attachToParentAt(lid, -0.3, 0, -1.5);

	milk = new sweep("milkprofile.txt", 20);
	milk->useSmoothShading();
	milk->setRotation('z', 90);
	milk->attachToParentAt(insideface, 0, -0.7, 0);

	for(int i=0; i<10; i++)
	{			
		diamond[i]	= new sweep("diamondprofile.txt", 8);
		diamond[i]->useDiffuseShading();
		diamond[i]->setColour(0.73, 0.91, 0.98, 0.8);
		diamond[i]->setScale(0.1);
		diamond[i]->setRotation('x', 90);
		diamond[i]->attachToParentAt(insideface, i*0.3 - 1.35, 0.5, 1.2);
	}

	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




// -----------------------------------------------------------------------------------------
// hide & show
// -----------------------------------------------------------------------------------------
// these methods override those of object3d, and cascade the hide and show message to
// all of the component parts
// -----------------------------------------------------------------------------------------
void composite1::hide()
{
	for(int i=0; i<10; i++)
	{			
		diamond[i]->hide();
	}

	outface->hide();
	insideface->hide();
	milk->hide();

	h_frame1->hide();
	h_frame2->hide();

	v_frame1->hide();
	v_frame2->hide();
	v_frame3->hide();
	v_frame4->hide();
	
	lid->hide();
	lid_frame1->hide();
	lid_frame2->hide();
	lid_frame3->hide();

	pivot->hide();
}

void composite1::show()
{
	for(int i=0; i<10; i++)
	{			
		diamond[i]->show();
	}

	outface->show();
	insideface->show();
	milk->show();

	h_frame1->show();
	h_frame2->show();

	v_frame1->show();
	v_frame2->show();
	v_frame3->show();
	v_frame4->show();
	
	lid->show();
	lid_frame1->show();
	lid_frame2->show();
	lid_frame3->show();

	pivot->show();
}


// -----------------------------------------------------------------------------------------
// openbox & closebox
// -----------------------------------------------------------------------------------------
// these methods used to do some action
// -----------------------------------------------------------------------------------------
void composite1::openbox(float time)
{
	int int_time = int(time);
	pivot->setRotation('x', int_time % 10 * (-10));
}

void composite1::closebox(float time)
{
	int int_time = int(time);
	pivot->setRotation('x', int_time % 10 * 10);
}