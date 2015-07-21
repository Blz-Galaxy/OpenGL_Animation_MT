// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// extrusion.h
//
// This file declares the class for extrusion objects, based on the
// generic 3D object


#ifndef _EXTRUSION_H
#define _EXTRUSION_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "face.h"
#include "object3d.h"



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class extrusion : public object3d
{
	private:
				int			plotCommands;
				int			profileVertexCount;
	
	public:
							extrusion(string profileFilename, string pathFilename);

};



#endif // _EXTRUSION_H

