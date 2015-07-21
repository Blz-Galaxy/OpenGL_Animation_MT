// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// opentorus.h
//
// Modified by Evan
//
// This file declares the class for opentorus objects, based on the
// generic 3D object


#ifndef _OPENTORUS_H
#define _OPENTORUS_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class opentorus : public object3d
{
	public:
		opentorus(int density, int slices, float radius, float height);

};



#endif // _OPENTORUS_H


