// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// extrusion.cpp
//
// =========================================================================================
//
// The constructor takes two parameters
// - the name of the file containing the extrusion profile (a list of xy pairs)
// - the name of the file containing the extrusion path (a list of string commands, see below)
//
// =========================================================================================
//
// This file defines the class for extrusion objects.  Extrusions are specified via a 2D
// profile, and a "path" which is a series of commands that tell the extrusion engine
// how to manipulate the profile as it is swept through space to generate the vertices
// of the extrusion.
//
// Profiles and paths are both stored in separate text files, which must be stored in
// the resources folder.  There is only one profile or path per file.
//
// The path commands are an identifying character followed by appropriate parameters.  
//
// Current commands and parameters are:
//
//	x r p			rotate the profile r degrees about an x-parallel axis at y=p
//	y r p			rotate the profile r degrees about a y-parallel axis at x=p
//	z r x y			rotate the profile r degrees about a z-parallel axis at x,y
//	m d				move the profile forward distance d
//	s x y			scale the profile by x, y
//	p				"print" - add the profile to the extrusion
//
// lines beginning with "//" and empty lines are ignored
//
// a simple path command might look like this:
//
//	p				add the profile as it is initially defined
//	m 1				push the profile forward (along the z axis) one unit
//	p				add the profile to the extrusion
//	s 0.2 0.2		scale the profile uniformly
//	m 1				push the profile one more unit into the z axis
//	p				add the profile to the extrusion
//
// all of the profiles added to the extrusion are "skinned" at the end of
// the command to generate a solid object



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "extrusion.h"
#include "utility.h"
#include "platform.h"
#include "matrix.h"




// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
extern vector<object3d*> gShapeVector;




// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------

extrusion::extrusion(string profileFilename, string pathFilename)
{	
	unsigned int k, v;
	string profileFileLocation, pathFileLocation, currentLine;
	ifstream theProfile, thePath;
	vector<float> profileX;
	vector<float> profileY;
	vector<string> commands;
	float x, y;
	char comma;

	
	setColour(0.38, 0.48, 0.74, 1.0);				// initialise a default colour
	setName("extrusion_" + profileFilename);

	// get the full path to the profile and the path files
	profileFileLocation = pathToResourceDirectory() + profileFilename;
	pathFileLocation = pathToResourceDirectory() + pathFilename;


	// read the profile into local arrays (vectors) and count the number of vertices on the profile
	profileVertexCount=0;
	theProfile.open(profileFileLocation.c_str());
	if (!theProfile)
	{
		string errMesg = "Unable to open extrusion profile ";
		errMesg.append(profileFilename);
		fatal (errMesg);
	}
	while (!theProfile.eof())
	{
		// read a line from the text file
		getline(theProfile, currentLine);
		
		// initialise this as a string stream for tokenisation
		stringstream ss(currentLine);
		
		string firstToken;
		ss >> firstToken;
		
		if ((firstToken.length() > 0) && (firstToken.substr(0, 2).compare("//")))		// ignore comment and empty lines
		{
			ss.seekg(0);						// rewind to start of token
			ss >> x >> comma >> y;				// read the radius (x) and y values from the profile
			if (comma != ',')
			{
				string errMesg = "Missing comma in extrusion profile ";
				errMesg.append(profileFilename);
				fatal (errMesg);
			}
			profileX.push_back(x);				// save the profile values in our local vectors
			profileY.push_back(y);
			profileVertexCount++;
		}
	}
	theProfile.close();
	

	
	// read the profile commands in to a string vector, and while we're at it, count
	// the number of "print" and "move" commands
	plotCommands=0;
	int matrixStacks=0;
	thePath.open(pathFileLocation.c_str());
	if (!thePath)
	{
		string errMesg = "Unable to open extrusion path ";
		errMesg.append(pathFilename);
		fatal (errMesg);
	}
	while (!thePath.eof())
	{
		// read a line from the text file
		getline(thePath, currentLine);
		
		// initialise this as a string stream for tokenisation
		stringstream ss(currentLine);
				
		string firstToken;
		ss >> firstToken;
		
		if ((firstToken.length() > 0) && (firstToken.substr(0, 2).compare("//")))		// ignore comment and empty lines
		{
			// find the first non white space character -> lastPos
			string::size_type lastPos = currentLine.find_first_not_of(" \t", 0);
			
			// find the first delimiter past the end of the command -> pos
			string::size_type pos = currentLine.find_first_of(",", 0);

			while (string::npos != pos || string::npos != lastPos)
			{
				// found a token, add it to the vector
				commands.push_back(currentLine.substr(lastPos, pos - lastPos));
				if (!currentLine.substr(lastPos, 1).compare("p"))
					plotCommands++;
				else 
					matrixStacks++;			// anything that is not a "print" command means another matrix

				// skip lastPos past the delimiter (to the next token)
				lastPos = currentLine.find_first_not_of(", \t", pos);
				
				// skip pos to the next delimiter past the next token, rinse & repeat
				pos = currentLine.find_first_of(",", lastPos);
				
			}
		}
	}
	thePath.close();
	


	// allocate space for the matrix stack - this will hold a matrix for every move and
	// rotate command that we encounter, since we need to apply (in reverse order) every
	// matrix that we've seen to date to the profile to build up the extrusion
	matrix *matrixStack = new matrix[matrixStacks];
	int matrixStackPtr = 0;

	
	// we can now calculate the total number of vertices
	vertexCount = profileVertexCount * plotCommands;



	// allocate space for a temporary copy of the profile that we can modify
	vectr *profileCopy = new vectr[profileVertexCount];
	


	// allocate memory for the vertex list
	vertices.resize(vertexCount);



	v = 0;												// init the vertex counter	
	
	for (unsigned i=0; i < commands.size(); i++)
	{
		stringstream theCommand(commands[i]);					// get the next command as a string-stream 
		matrix a, b, c, d;
		float rot = 0, pivot = 0, pivot2 = 0, sx = 0, sy = 0, dist = 0;					// temporary vars
		char firstChar;
		
		theCommand >> firstChar;									// examine just the first character
		switch (firstChar)
		{
			case 's':											// SCALE command
				theCommand >> sx >> sy;								// get the scale values
				d.setScale(sx, sy, 1.0);							// turn them into a scale matrix
				matrixStack[matrixStackPtr++] = d;					// push it onto the matrix stack
				break;


			case 'x' : 											// X-AXIS ROTATION
				theCommand >> rot >> pivot;							// get the angle and pivot point
				a.setTranslate(0, -pivot, 0);
				b.setRotateX(rot);
				c.setTranslate(0, pivot, 0);
				d = c * b;											// build a composite matrix to do the
				d = d * a;											// rotation and stack it
				matrixStack[matrixStackPtr++] = d;

				break;


			case 'y' : 											// Y-AXIS ROTATION
				theCommand >> rot >> pivot;							// get the angle and pivot point
				a.setTranslate(-pivot, 0, 0);
				b.setRotateY(rot);
				c.setTranslate(pivot, 0, 0);
				d = c * b;											// build a composite matrix to do the
				d = d * a;											// rotation and stack it
				matrixStack[matrixStackPtr++] = d;

				break;


			case 'z' : 											// Z-AXIS ROTATION
				theCommand >> rot >> pivot >> pivot2;				// get the angle and pivot point
				a.setTranslate(-pivot, -pivot2, 0);
				b.setRotateZ(rot);
				c.setTranslate(pivot, pivot2, 0);
				d = c * b;											// build a composite matrix to do the
				d = d * a;											// rotation and stack it
				matrixStack[matrixStackPtr++] = d;

				break;


			case 'm' : 											// MOVE
				theCommand >> dist;									// get the distance
				d.setTranslate (0, 0, dist);						// build a matrix to implement it
				matrixStack[matrixStackPtr++] = d;					// stack it
				break;


				
			case 'p' : 											// PLOT THE PROFILE
			
				for (int i=0; i<profileVertexCount; i++)			// copy the original profile
					profileCopy[i].set(profileX[i], profileY[i], 0);
					
				for (int i=matrixStackPtr; i>0; i--)				// work backwards through the matrix stack
					for (int j=0; j<profileVertexCount; j++)		// applying each matrix to every profile point
						profileCopy[j] = matrixStack[i-1] * profileCopy[j];
				
				for (int i=0; i<profileVertexCount; i++)			// add the transformed profile to the extruded object
				{
					for (int j=0; j<4; j++)
						vertices[v].coordinate.data[j] = profileCopy[i].data[j];
					v++;
				}
					
				break;
		}


	}
	

	// check that we use all the slots we created in the vertex array
	if (v > vertexCount) fatal("generated more vertices than you said you would");
	if (v < vertexCount) fatal("allocated space for more vertices than you generated");




	// allocate memory for the face list
	faceCount = (profileVertexCount * (plotCommands -1)) * 2;
	faces.resize(faceCount);
	polygonCount = faceCount;


	// link the faces
	// note that we use triangles, not quads for this because it's possible that some quads will
	// be twisted (ie., not all vertices will lie on a plane) depending on the path
	
	k=0;											// face counter (for half of the faces)
	
	for (int i=0; i < plotCommands-1; i++)
	{
		int offset1 = profileVertexCount * i;
		int offset2 = offset1 + profileVertexCount;
		for (int j=0; j < profileVertexCount; j++)
		{
			faces[k++].init(offset1 + j,
							offset1 + ((j+1) % profileVertexCount),
							offset2 + ((j+1) % profileVertexCount));
			faces[k++].init(offset1 + j,
							offset2 + ((j+1) % profileVertexCount),
							offset2 + j);
		}
	}



	// check that we use all the slots we created in the face array
	if (k > faceCount) fatal("generated more faces than you said you would");
	if (k < faceCount) fatal("allocated space for more faces than you generated");


	// calculate the face and vertex normals
	calculateNormals();	

	
	// don't need the profiles any more
	delete [] profileCopy;
	delete [] matrixStack;

	
	// put the shape onto the shapeVector so it gets draw messages
	gShapeVector.push_back(this);
}




