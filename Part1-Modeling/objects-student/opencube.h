// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// opencube.h
//
// Modified by Evan
//
// This file declares the class for opencube objects, based on the
// generic 3D object


#ifndef _OPENCUBE_H
#define _OPENCUBE_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------

class opencube : public object3d
{
	public:
		opencube();
};


#endif // _OPENCUBE_H

