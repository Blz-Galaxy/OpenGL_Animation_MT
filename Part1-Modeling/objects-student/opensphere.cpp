// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// opensphere.cpp
//
// Modified by Evan
//
// This file defines the class for opensphere objects



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "opensphere.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;



// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------

opensphere::opensphere(int density)
{

	setColour(1.0, 0.0, 0.0);					// default colour (red)
	setName("opensphere");

	// if the user passes an odd number as the "density" of the opensphere,
	// round this up to the next even number. 
	if(density%2 == 1)
	{
		density++;
	}
	
	// calculate the number of vertices and faces from the mesh density
	vertexCount = (density * (density-1)) + 2;
	faceCount = (density * (density - 2)) + (density * 2);
	polygonCount = faceCount;


	// allocate memory for the vertex and face arrays
	vertices.resize(vertexCount);
	faces.resize(faceCount);
	


	// generate the vertex information to build a 2-unit opensphere centred on the origin
	float stepSize = 180.0 / density;				// rotating around Z axis by this amount
	float zRot = 90.0 - stepSize;					// starting Z rot near top;
	unsigned int v = 0;								// init the vertex counter
	
	for (int i=1; i<density; i++)					// outer loop (Z Rot)
	{
		for (int j=0; j<density; j++)				// inner loop (Y rot)
		{
			// calculate angle around Y axis
			float yRot = j * (360.0 / density);
			
			// calculate x, y & z coordinates
			float x = cos_d(zRot) * cos_d(yRot);
			float z = cos_d(zRot) * sin_d(yRot);
			float y = sin_d(zRot);
			
			vertices[v++].set(x, y, z);				// init this vertex

		}
		
		zRot = zRot - stepSize;						// next row down
	}
	
	vertices[v++].set(0,  1, 0);					// the top vertex
	vertices[v++].set(0, -1, 0);					// the bottom vertex


	// this first test may never execute if we've overflowed the array (since the program will have crashed by now)
	if (v > vertexCount)
		fatal("generated more vertices than you said you would");
	if (v < vertexCount)
		fatal("allocated space for more vertices than you generated");




	// now set up the faces
	unsigned int k=0;											// face counter

	
	// first define the sides as quads
	for (int i=0; i < density-2; i++)
	{
		int offset1 = (density * i);
		int offset2 = (offset1 + density);
		for (int j=0; j < density; j+=2)
		{
			// external face
			faces[k++].init(offset1 + j, offset1 + ((j+1) % density), offset2 + ((j+1) % density), offset2 + j);

			// internal face (reverse the order of vertexes)
			faces[k++].init(offset2 + j, offset2 + ((j+1) % density), offset1 + ((j+1) % density), offset1 + j);
		}
	}

	
	// next define the top triangles
	for (int i=0; i < density; i+=2)
	{
		// external face
		faces[k++].init((density * (density - 1)), (i+1) % density, i); 
		
		// internal face (reverse the order of vertexes)
		faces[k++].init(i, (i+1) % density, (density * (density - 1))); 
	}

	
	// finally define the bottom triangles
	for (int i=0; i < density; i+=2)
	{
		// external face
		faces[k++].init(	(density * (density -2)) + i,
							(density * (density -2)) + ((i+1) % density),
							(density * (density -1)) +1);

		// internal face (reverse the order of vertexes)
		faces[k++].init(	(density * (density -1)) +1,
							(density * (density -2)) + ((i+1) % density),
							(density * (density -2)) + i);
	}

	// this first test may never execute if we've overflowed the array (since the program will have crashed by now)
	if (k > faceCount) fatal("generated more faces than you said you would");
	if (k < faceCount) fatal("allocated space for more faces than you generated");


	// calculate the face and vertex normals
	calculateNormals();

	// set the default shading model of the opensphere to diffuse
	diffuseShading = true;
	
	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}



