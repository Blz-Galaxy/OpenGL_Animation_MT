// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// torus.h
//
// This file declares the class for torus objects, based on the
// generic 3D object


#ifndef _torus_H
#define _torus_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class torus : public object3d
{
	public:
						torus(int density, int slices, float radius, float height);

};



#endif // _torus_H


