// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// sweep.h
//
// This file declares the class for sweep objects, based on the
// generic 3D object


#ifndef _sweep_H
#define _sweep_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class sweep : public object3d
{
	private:
				int	profilePoints;			// set during construction to the no of points that 
											// describe the profile of the sweepd shape


	public:
				sweep(string theFilename, int resolution);

};


#endif // _sweep_H

