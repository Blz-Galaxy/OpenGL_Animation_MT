// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// camera.h
//
// This file declares a class for an OpenGL camera

#ifndef _CAMERA_H
#define _CAMERA_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "vectr.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class cameraClass
{
	protected:
		vectr		position;                                       // where the camera sits in world space
		vectr		target;                                         // where the camera is looking
		vectr		upVector;                                       // which way is "up" for the camera

		
	public:
					cameraClass();
		virtual		~cameraClass();

		void		setPosition(float x, float y, float z);         // set the camera's world location matrix
		void		setUpVector(float X, float y, float z);			// set the camera's up vector
		void		moveBy(float X, float y, float z);				// move the camera by a fixed amount
		void		setTarget(object3d *);							// target the camera at an object
		void		setTarget(float x, float y, float z);			// target the camera at a point
		void		moveTargetBy(float x, float y, float z);		// move where the camera looks
		
		void		getPosition(float *x, float *y, float *z);      // get the camera's location
		void		getPosition(vectr &location);                   // get the camera's location vector
		vectr		getPosition(void);                              // get the camera's location vector
		void		getTarget(vectr &location);                     // get the camera's target location vector
		
		void		tellGL();										// send the camera data to OpenGL
		void		tellGLCentered();								// send the camera data to OpenGL (special skybox version)
		
};


#endif // _CAMERA_H

