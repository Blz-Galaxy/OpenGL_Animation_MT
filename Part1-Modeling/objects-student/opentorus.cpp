// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// opentorus.cpp
//
// Modified by Evan
//
// This file defines the class for opentorus objects



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "opentorus.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;



// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------

opentorus::opentorus(int density, int slices, float radius, float height)
{

	setColour(1.0, 1.0, 0.0);					// default colour (yellow)
	setName("opentorus");
	
	// if the user passes an odd number as the "slices" of the opentorus,
	// round this up to the next even number. 
	if(slices%2 == 1)
	{
		slices++;
	}

	// calculate the number of vertices and faces from the mesh density
	vertexCount = density * slices;
	faceCount = density * slices;
	polygonCount = faceCount;
	

	// allocate memory for the vertex and face arrays
	vertices.resize(vertexCount);
	faces.resize(faceCount);
	

	float yStepSize = 360.0 / slices;						// rotating around Y axis by this amount
	float zStepSize = 360.0 / density;						// rotating around Z axis by this amount


	// generate the vertex information
	unsigned int v = 0;										// init the vertex counter
	float zRot = 0;
	
	for (int i=0; i<density; i++)							// outer loop (Z Rot)
	{
		// calculate angle around Z axis - this is the "height" of the horizontal
		// disc, and the Y coordinate of all vertices on the circumference of that disc
		float y = height * sin_d(zRot);

		// also calculate X - this is subtracted from the opentorus radius to
		// calculate the radius for the horizontal disc
		float x = height * cos_d(zRot);
		float discRad = radius - x;
		float yRot=0;
		
		// OK - now split the disc into slices
		for (int j=0; j<slices; j++)						// inner loop (Y rotation)
		{
			vertices[v++].set(discRad * cos_d(yRot), y, discRad * sin_d(yRot));	
			yRot+=yStepSize;								// step around for the next division
		}
		
		zRot+=zStepSize;									// step around for the next disc
	}

	// this first test may never execute if we've overflowed the array (since the program will have crashed by now)
	if (v > vertexCount) fatal("generated more vertices than you said you would");
	if (v < vertexCount) fatal("allocated space for more vertices than you generated");




	// now generate the face list
	unsigned int k=0;													// face counter
	
	for (int zLoop=0; zLoop<density; zLoop++)							// loop for each disc
	{
		int offset1 = (zLoop * slices);									// this is the "first vertex" for this disc
		int offset2 = ((offset1 + slices)% (slices * density));			// this is the "first vertex" of the NEXT disc
		for (int yLoop=0; yLoop<slices; yLoop+=2)						// loop around each disc
		{
			// external face
			faces[k++].init(offset1 + yLoop, 							// the first vertex of this disc
								offset1 + ((yLoop+1)%slices),			// the next is on this disc, one around
								offset2 + ((yLoop+1)%slices),			// the next is on next disc, one around
								offset2 + yLoop); 						// the last is on the next disc, below first 


			// internal face (reverse the order of vertexes)
			faces[k++].init(offset2 + yLoop, 							// the last is on the next disc, below first
								offset2 + ((yLoop+1)%slices),			// the next is on next disc, one around	
								offset1 + ((yLoop+1)%slices),			// the next is on this disc, one around
								offset1 + yLoop); 						// the first vertex of this disc
		}
	}

	// this first test may never execute if we've overflowed the array (since the program will have crashed by now)
	if (k > faceCount) fatal("generated more faces than you said you would");
	if (k < faceCount) fatal("allocated space for more faces than you generated");



	// calculate the face and vertex normals
	calculateNormals();	

	// set the default shading model of the opentorus to diffuse
	diffuseShading = true;
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}



