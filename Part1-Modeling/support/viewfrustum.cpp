// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// viewfrustum.cpp
//
// This file defines a class for managing viewfrustum objects
//
// viewfrustum f = new viewfrustum(fieldOfView, aspectRatio, zNear, zFar);
//
// normally, there is only one frustum, which can be manipulated at runtime


// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "viewfrustum.h"




// -----------------------------------------------------------------------------------------
// externals
// -----------------------------------------------------------------------------------------
extern int		gWindowWidth;						// the current window size
extern int		gWindowHeight;




// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------
// if a viewfrustum is constructed with no parameters, default all values
// -----------------------------------------------------------------------------------------

viewfrustum::viewfrustum()
{
    vfFov = 50.0;
    vfZNear = 0.1;
    vfZFar = 500.0;
}




// -----------------------------------------------------------------------------------------
// viewfrustum constructor when all parameters specified
// -----------------------------------------------------------------------------------------

viewfrustum::viewfrustum(double fov, double znear, double zfar)
{
    vfFov = fov;
    vfZNear = znear;
    vfZFar = zfar;
}




// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------

viewfrustum::~viewfrustum()
{
}




// -----------------------------------------------------------------------------------------
// tellGL
// -----------------------------------------------------------------------------------------
// send the viewfrustum details to OpenGL
// -----------------------------------------------------------------------------------------

void viewfrustum::tellGL()
{
	glLoadIdentity();
	gluPerspective(vfFov, (float)gWindowWidth/(float)gWindowHeight, vfZNear, vfZFar);					
}					





