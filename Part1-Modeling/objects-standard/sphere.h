// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// sphere.h
//
// This file declares the class for sphere objects, based on the
// generic 3D object


#ifndef _SPHERE_H
#define _SPHERE_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class sphere : public object3d
{
	public:
						sphere(int density);

};


#endif // _SPHERE_H


