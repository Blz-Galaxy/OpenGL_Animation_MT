// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// cube.cpp
//
// This file defines the class for cube objects



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "cube.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;



// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

cube::cube()
{
	// Initialise the object's state
	setName("cube");
	setColour(0.15, .85, .35, 1.0);					// default colour

	vertexCount = 8;
	faceCount = 6;
	polygonCount = 6;

	// allocate memory for the vertex and face arrays
	vertices.resize(vertexCount);
	faces.resize(faceCount);


	// Initialise the cube's vertices to create a cube centered around the origin
	vertices[0].set(-1,  1,  1);				// front, top, left
	vertices[1].set( 1,  1,  1);				// front, top, right
	vertices[2].set( 1, -1,  1);				// front, bot, right
	vertices[3].set(-1, -1,  1);				// front, bot, left
	vertices[4].set(-1,  1, -1);				// back,  top, left
	vertices[5].set( 1,  1, -1);				// back,  top, right
	vertices[6].set( 1, -1, -1);				// back,  bot, right
	vertices[7].set(-1, -1, -1);				// back,  bot, left


	// now set up the faces - note that the vertex order is always
	// specified counter-clockwise when that face is viewed front on
	faces[0].init(0, 3, 2, 1);					// front
	faces[1].init(1, 2, 6, 5);					// right
	faces[2].init(5, 6, 7, 4);					// back
	faces[3].init(4, 7, 3, 0);					// left
	faces[4].init(4, 0, 1, 5);					// top
	faces[5].init(3, 7, 6, 2);					// bottom
	
	
	// calculate the face and vertex normals
	calculateNormals();
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}



