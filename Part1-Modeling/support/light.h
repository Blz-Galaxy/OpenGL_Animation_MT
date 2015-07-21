// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// light.h
//
// This file declares a class for OpenGL lights.  A light object is derived from an
// object3d (since some of the fields are useful to inherit).
// It also contains a class function for dealing with a list (STL vector) of lights


#ifndef _LIGHT_H
#define _LIGHT_H

#include "object3d.h"



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class light : public object3d
{
	protected:
				bool	on;							// true = light is on
				bool	lightType;					// kPositionalLight (true) or kDirectionalLight (false)
				GLshort	lightID;					// the ID of the light in OpenGL
													// (GL_LIGHT0, GL_LIGHT1, etc.)
													// or GL_LIGHT_MODEL_AMBIENT for ambient light
													
				float	constantAttenuation;
				float	linearAttenuation;
				float	quadraticAttenuation;
				float	spotCutoff;
				vectr	spotDirection;

				void	tellGL();						// send this light to the OpenGL pipeline
													
	public:
						light(int lightID);
		virtual			~light();

				void	turnOn() { on = true; }
				void	turnOff() { on = false; }
				bool	isOn() { return on; }
				
				void	makePositional() { lightType = kPositionalLight; }
				void	makeDirectional() { lightType = kDirectionalLight; }

				void	setSpotCutoff(float c) { spotCutoff = c; }
				
				void	setSpotDirection(float x, float y, float z);
				void	setSpotDirection(vectr &dir);
				void	getSpotDirection(float &x, float &y, float &z);
				vectr	getSpotDirection(void);
				
				void	setConstantAttenuation(float a)	{ constantAttenuation = a; }
				void	setLinearAttenuation(float a)	{ linearAttenuation = a; }
				void	setQuadraticAttenuation(float a) { quadraticAttenuation = a; }

		static	void	tellAllGL(void);				// send all lights to OpenGL (class function)
		static	void	drawAll(void);					// draw proxy objects for all lights

		virtual void	privateDraw(bool drawMode);		

};


#endif // _LIGHT_H


