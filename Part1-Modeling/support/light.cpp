// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// light.cpp
//
// This file defines a class for OpenGL lights, but also defines
// a class function for dealing with a list (STL vector) of lights
// (so the state of all lights can be passed to OpenGL)
// it also declares the global light vector




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "light.h"
#include "utility.h"
#include "camera.h"



// -----------------------------------------------------------------------------------------
// global variables
// -----------------------------------------------------------------------------------------
extern cameraClass		gCamera;




// -----------------------------------------------------------------------------------------
// private bglobals
// -----------------------------------------------------------------------------------------
// define a global vector of light object pointers, and an iterator for them
// so we can walk the list of lights 
// -----------------------------------------------------------------------------------------
vector<light*> lightVector;
typedef vector<light*>::iterator lightVectorIterator;





// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

light::light(int oglLightID)
{
	setName("light");
	setPosition(0.0, 20.0, 0.0);					// default location of light
	setColour(0.5, 0.5, 0.5, 1.0);					// default colour (grey)
	setSpecularColour(0.5, 0.5, 0.5, 1.0);			// default specular colour (grey)
	parent = 0;										// no parent
	on = false;										// lights are initially off
	spotCutoff = 180.0;								// this means the light is NOT a spotlight
	makeDirectional();								// by default, lights are directional (stops odd lighting on adjacent co-planar faces)
	
	constantAttenuation = 1.0;						// set defult attenuation for positional lights
	linearAttenuation = 0;							// (using OpenGL defaults)
	quadraticAttenuation = 0;
	
	lightID = oglLightID;
	
	lightVector.push_back(this);

}



// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------
// ensures this light is removed from the light vector, before it is deleted
// -----------------------------------------------------------------------------------------

light::~light()
{
	lightVectorIterator lvi;
	for (lvi = lightVector.begin(); lvi != lightVector.end(); lvi++)
		if (*lvi == this)
			lightVector.erase(lvi);
}




// -----------------------------------------------------------------------------------------
// setSpotDirection, getSpotDirection
// -----------------------------------------------------------------------------------------
// get/set the material colour of the object
// -----------------------------------------------------------------------------------------
void light::setSpotDirection(float x, float y, float z)	
{
	spotDirection.set(x, y, z, 1.0);
}

void light::setSpotDirection(vectr &dir)	
{
	spotDirection = dir;
}

void light::getSpotDirection(float &x, float &y, float &z)	
{
	x = spotDirection.data[0];
	y = spotDirection.data[1];
	z = spotDirection.data[2];
}

vectr light::getSpotDirection(void)
{
	return spotDirection;
}




// -----------------------------------------------------------------------------------------
// tellGL
// -----------------------------------------------------------------------------------------
// send a single light's characteristics to OpenGL
// -----------------------------------------------------------------------------------------

void light::tellGL()
{
	if (lightID == GL_LIGHT_MODEL_AMBIENT)
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, theColour);
	}
	else
	{
		if (on)
		{
			glEnable(lightID);
			if (parent)	buildModelViewMatrix(0);
			if (lightType == kDirectionalLight)
				position.data[3] = 0;
			else
				position.data[3] = 1.0;
			glLightfv(lightID, GL_POSITION, position.data);
			glLightfv(lightID, GL_DIFFUSE, theColour);
			glLightfv(lightID, GL_SPECULAR, theSpecularColour);
			glLightf(lightID, GL_CONSTANT_ATTENUATION, constantAttenuation); 
			glLightf(lightID, GL_LINEAR_ATTENUATION, linearAttenuation);
			glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation); 
			glLightf(lightID, GL_SPOT_CUTOFF, spotCutoff);
			glLightfv(lightID, GL_SPOT_DIRECTION, spotDirection.data);
		}
		else
			glDisable(lightID);
	}
}					




// -----------------------------------------------------------------------------------------
// tellAllGL() - CLASS METHOD
// -----------------------------------------------------------------------------------------
// send all light's characteristics to OpenGL
// -----------------------------------------------------------------------------------------

void light::tellAllGL(void)
{
	lightVectorIterator lvi;
	for (lvi = lightVector.begin(); lvi != lightVector.end(); lvi++)
		(*lvi)->tellGL();
}					




// -----------------------------------------------------------------------------------------
// drawAll() - CLASS METHOD
// -----------------------------------------------------------------------------------------
// draw light proxies for all lights
// -----------------------------------------------------------------------------------------

void light::drawAll(void)
{
	lightVectorIterator lvi;
	for (lvi = lightVector.begin(); lvi != lightVector.end(); lvi++)
		if ((*lvi)->lightID != GL_LIGHT_MODEL_AMBIENT)
			(*lvi)->draw(kOpaqueMode);
}					





// -----------------------------------------------------------------------------------------
// custom draw method for lights
// -----------------------------------------------------------------------------------------
void light::privateDraw(bool drawMode)
{
	// don't draw a proxy for the ambient component!
	if (lightID == GL_LIGHT_MODEL_AMBIENT) return;
	
	if (on)
		glColor4f(1.0, 1.0, 0.0, 1.0);
	else
		glColor4f(0.3, 0.3, 0.3, 1.0);

	glutSolidSphere(0.3, 12, 12);
	glColor4f(1.0, 1.0, 1.0, 1.0);

}


