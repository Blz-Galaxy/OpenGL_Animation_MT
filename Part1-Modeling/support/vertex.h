// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// vertex.h
//
// This is a class used to represent a vertex.  The main requirement is that
// this class provide storage for a 4 row vector to represent a point in 3D space.
//
// Another data member usually required for vertices (and included here) is a vectr
// to represent the vertex normal.  This isn't always used - but is required when
// implementing smooth shading.


#ifndef _VERTEX_H
#define _VERTEX_H


#include "vectr.h"



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class vertex
{
	public:
				vectr	coordinate;						// the coordinate data for this vertex
				vectr	normal;							// the normal for this vertex (for lighting)
		
						vertex();						// default constructor
		virtual			~vertex();						// destructor

				void	set(float x, float y, float z)	// initialiser
						{ coordinate.set(x, y, z); }

};



#endif // _VERTEX_H

