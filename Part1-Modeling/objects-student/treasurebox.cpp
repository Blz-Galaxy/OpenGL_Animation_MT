// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Edited by Evan
//
// treasurebox.cpp
//
// Note:  this class makes changes on the basis of the opencube
//
// This file defines the class for treasurebox objects



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "treasurebox.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;



// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

treasurebox::treasurebox()
{
	// Initialise the object's state
	setName("treasurebox");
	setColour(0.15, .85, .35, 1.0);					// default colour

	vertexCount = 8;
	faceCount = 10;
	polygonCount = 10;

	// allocate memory for the vertex and face arrays
	vertices.resize(vertexCount);
	faces.resize(faceCount);


	// Initialise the treasurebox's vertices to create a treasurebox centered around the origin
	vertices[0].set(-1.5,  0.5,  1);			// front, top, left
	vertices[1].set( 1.5,  0.5,  1);			// front, top, right
	vertices[2].set( 1.3, -1,  1);				// front, bot, right
	vertices[3].set(-1.3, -1,  1);				// front, bot, left
	vertices[4].set(-1.5,  0.5, -1);			// back,  top, left
	vertices[5].set( 1.5,  0.5, -1);			// back,  top, right
	vertices[6].set( 1.3, -1, -1);				// back,  bot, right
	vertices[7].set(-1.3, -1, -1);				// back,  bot, left


	// now set up the faces - note that the vertex order is always
	// specified counter-clockwise when that face is viewed front on
	faces[0].init(0, 3, 2, 1);					// front
	faces[1].init(1, 2, 6, 5);					// right
	faces[2].init(5, 6, 7, 4);					// back
	faces[3].init(4, 7, 3, 0);					// left
	faces[4].init(3, 7, 6, 2);					// bottom

	// internal faces are clockwise, therefore reverse the order of vertexes
	faces[5].init(1, 2, 3, 0);					// front internal face
	faces[6].init(5, 6, 2, 1);					// right internal face
	faces[7].init(4, 7, 6, 5);					// back internal face
	faces[8].init(0, 3, 7, 4);					// left internal face
	faces[9].init(2, 6 ,7, 3);					// bottom internal face

	
	
	// calculate the face and vertex normals
	calculateNormals();
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}



