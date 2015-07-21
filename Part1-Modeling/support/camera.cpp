// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// camera.cpp
//
// This file defines a class for an OpenGL camera



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "camera.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------

cameraClass::cameraClass()
{
	setPosition(0.0, 0.0, 8.0);         // default location
	setTarget(0.0, 0.0, 0.0);			// default target (looking at the origin)
	setUpVector(0.0, 1.0, 0.0);			// default up vector (positive "y" axis is up)
}



// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------

cameraClass::~cameraClass()
{
}




// -----------------------------------------------------------------------------------------
// setCameraLocation
// -----------------------------------------------------------------------------------------

void cameraClass::setPosition(float x, float y, float z)
{
	position.set(x, y, z);
}




// -----------------------------------------------------------------------------------------
// setUpVector
// -----------------------------------------------------------------------------------------

void cameraClass::setUpVector(float x, float y, float z)
{
	upVector.set(x, y, z);
}




// -----------------------------------------------------------------------------------------
// moveCameraBy
// -----------------------------------------------------------------------------------------

void cameraClass::moveBy(float x, float y, float z)
{
	position.add(x, y, z);
}




// -----------------------------------------------------------------------------------------
// setCameraTarget
// -----------------------------------------------------------------------------------------

void cameraClass::setTarget(float x, float y, float z)
{
	target.set(x, y, z);
}




// -----------------------------------------------------------------------------------------
// setCameraTarget
// -----------------------------------------------------------------------------------------

void cameraClass::setTarget(object3d *theObject)
{
	target.set(theObject->getPosition());
}




// -----------------------------------------------------------------------------------------
// moveTargetBy
// -----------------------------------------------------------------------------------------

void cameraClass::moveTargetBy(float x, float y, float z)
{
	target.add(x, y, z);
}




// -----------------------------------------------------------------------------------------
// getPosition
// -----------------------------------------------------------------------------------------

void cameraClass::getPosition(float *x, float *y, float *z)
{
	*x = position.data[0];
	*y = position.data[1];
	*z = position.data[2];
}




// -----------------------------------------------------------------------------------------
// getPosition (overloaded)
// -----------------------------------------------------------------------------------------

void cameraClass::getPosition(vectr &location)
{
	location = position;
}




// -----------------------------------------------------------------------------------------
// getPosition (overloaded)
// -----------------------------------------------------------------------------------------

vectr cameraClass::getPosition(void)
{
	return position;
}




// -----------------------------------------------------------------------------------------
// getTarget
// -----------------------------------------------------------------------------------------

void cameraClass::getTarget(vectr &location)
{
	location.set(
		target.data[0],
		target.data[1],
		target.data[2]);
}




// -----------------------------------------------------------------------------------------
// tellGL
//
// builds a view transform (using glu) based on the camera's position,
// target, and up vector
// -----------------------------------------------------------------------------------------

void cameraClass::tellGL()
{
	glLoadIdentity();
	gluLookAt(	position.data[0], position.data[1], position.data[2],
				target.data[0], target.data[1], target.data[2],
				upVector.data[0], upVector.data[1], upVector.data[2]);
}




// -----------------------------------------------------------------------------------------
// tellGLCentered
//
// builds a view transform (using glu) but with the camera at the origin - used
// for drawing skyboxes
// -----------------------------------------------------------------------------------------

void cameraClass::tellGLCentered()
{
	glLoadIdentity();
	gluLookAt(	0.0, 0.0, 0.0,
				target.data[0] - position.data[0], target.data[1] - position.data[1], target.data[2] - position.data[2],
				upVector.data[0], upVector.data[1], upVector.data[2]);
}





