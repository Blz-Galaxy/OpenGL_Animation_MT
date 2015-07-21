// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// face.h
//
// This file declares a face class
//
// A face consists of three or four vertices.  The vertices themselves aren't
// stored in this class - they are stored in a separate "vertexlist" array,
// a pointer to which is stored in the object3d class.  What we store
// here in place of the vertex objects is their position in that array.


#ifndef _FACE_H
#define _FACE_H

#include "vectr.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class face
{
	public:

				unsigned int vertices;							// the number of vertices for this face (3 or 4)
				unsigned int vert[4];							// indices for the three (or four) vertices in the vertex vector
				vectr	faceNormal;								// the normal of this face (for lighting)
				vectr	centroid;								// centre of the face (for depth sorting)
				float	distanceToCamera;						// the distance from this face's centroid to the camera

						face();									// default constructor
		virtual			~face();								// destructor


				void	init(unsigned int a, unsigned int b, unsigned int c);					// initialise a new face with 3 sides
				void	init(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// initialise a new face with 4 sides
		
				unsigned int vertex(unsigned int n);			// return the position of vertex n in the
																// vertexlist array (n=0-3)
};


#endif // _FACE_H

