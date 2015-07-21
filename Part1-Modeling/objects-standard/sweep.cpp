// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// sweep.cpp
//
// This file defines the class for a sweep object



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "sweep.h"
#include "platform.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;



// -----------------------------------------------------------------------------------------
// constructor when a profile is specified
// -----------------------------------------------------------------------------------------

sweep::sweep(string theFilename, int density)
{
	unsigned int k, v;
	unsigned int offset1, offset2;
	float	yRot;								// sweeper around Y axis (inner loop)	
	float	radius, x, y, z;					// temporary coordinate holders
	
	string fileLocation, currentLine;
	char comma;
	ifstream theFile;


	// get the full path to the sweep profile
	fileLocation = pathToResourceDirectory() + theFilename;
	setName("sweep_" + theFilename);


	theFile.open(fileLocation.c_str());
	if (!theFile)
	{
		string errMesg = "Unable to open sweep profile ";
		errMesg.append(theFilename);
		fatal (errMesg);
	}
	v = 0;												// v tracks the next available vertex
	profilePoints = 0;									// tracks points on the profile
	while (!theFile.eof())
	{
		// read a line from the text file
		getline(theFile, currentLine);
		
		// initialise this as a string stream for tokenisation
		stringstream ss(currentLine);
		
		string firstToken;
		ss >> firstToken;

		if ((firstToken.length() > 0) && (firstToken.substr(0, 2).compare("//")))		// ignore comment & empty lines
		{
			ss.seekg(0);								// rewind to start of token
			ss >> radius >> comma >> y;					// read the radius (x) and y values from the profile
			if (comma != ',')
			{
				string errMesg = "Missing comma in sweep profile ";
				errMesg.append(theFilename);
				fatal (errMesg);
			}
		
			profilePoints++;
			
			// generate the vertex information to build a sweepd object centred on the origin
			for (int i=0; i<density; i++)			// inner loop (sweeping around the Y axis for this disc)
			{
				// calculate angle around Y axis
				yRot = i * (360.0 / density);
				
				// calculate x, y & z coordinates
				x = radius * cos_d(yRot);
				z = radius * sin_d(yRot);
				
				vertices.resize(v+1);					// ensure space for this many vertices
				vertices[v++].set(x, y, z);				// init this vertex
			}
		}
	}
	theFile.close();
	

	vertexCount = v;
	polygonCount = faceCount = density * (profilePoints-1);
	faces.resize(faceCount);


	// now set up the faces - note that the vertex order is always
	// specified clockwise when that face is viewed front on
	
	k=0;											// init face counter
	
	for (int i=0; i < profilePoints-1; i++)
	{
		offset1 = (density * i);
		offset2 = (offset1 + density);
		for (int j=0; j < density; j++)
			faces[k++].init(offset1 + j, offset1 + ((j+1)%density), offset2 + ((j+1)%density), offset2 + j);
	}
	

	// calculate the face and vertex normals
	calculateNormals();



	// ------------------------------------------------------------------------------------
	// now get ready to record the shape into an OpenGL display list
	// ------------------------------------------------------------------------------------

	listName = glGenLists(1);											// generate a display list index
	if (listName == 0)
		fatal("sweep constructor couldn't record shape into an OpenGL list");


	// start recording into the list
	glNewList(listName, GL_COMPILE);									// create the list

	// draw the polygons as quad strips
	for (int j=0; j<profilePoints-1; j++)								// loop for each disc
	{
		int offset1 = j * density;										// this is the "first vertex" for this row of strips
		int offset2 = offset1 + density;								// this is the "first vertex" of the NEXT row of strips
		glBegin(GL_QUAD_STRIP);

		for (int i=0; i<density; i++)									// loop around each disc
		{
			glNormal3fv(vertices[offset2 + i].normal.data);
			glVertex3fv(vertices[offset2 + i].coordinate.data);
			glNormal3fv(vertices[offset1 + i].normal.data);
			glVertex3fv(vertices[offset1 + i].coordinate.data);
		}

		// close the gap at the end of the strip
		glNormal3fv(vertices[offset2].normal.data);
		glVertex3fv(vertices[offset2].coordinate.data);
		glNormal3fv(vertices[offset1].normal.data);
		glVertex3fv(vertices[offset1].coordinate.data);
		glEnd();
	}		

	glEndList();

	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




