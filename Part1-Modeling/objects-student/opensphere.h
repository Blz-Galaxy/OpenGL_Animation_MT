// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// opensphere.h
//
// Modified by Evan
//
// This file declares the class for opensphere objects, based on the
// generic 3D object


#ifndef _OPENSPHERE_H
#define _OPENSPHERE_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class opensphere : public object3d
{
	public:
		opensphere(int density);

};


#endif // _OPENSPHERE_H


