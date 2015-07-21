// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// utility.cpp
//
// contains general utility functions for the program




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "utility.h"
#include "constants.h"
#include "camera.h"
#include "platform.h"



// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern int				gWindowWidth;			// the current window size
extern int				gWindowHeight;
extern long				gRenderedFrameCounter;	// the number of frames we've rendered
extern long				gAnimatedFrameCounter;	// the number of frames we've animated
extern ProgramMode		gProgramMode;			// the current operating mode of the program
extern float			gCurrentFrameTime;		// the time that the current animation Frame was started
extern cameraClass		gCamera;




// -----------------------------------------------------------------------------------------
// local constants
// -----------------------------------------------------------------------------------------
const float degPerRad = 180/3.141592654;		// the number of degrees per radian





// -----------------------------------------------------------------------------------------
// randomFloat()
// -----------------------------------------------------------------------------------------
// returns a random float between 0.0 and 1.0
// -----------------------------------------------------------------------------------------

float randomFloat()
{
	return ((double)rand() / RAND_MAX);
}





// -----------------------------------------------------------------------------------------
// randomFloat
// -----------------------------------------------------------------------------------------
// returns a random float between min and max
// -----------------------------------------------------------------------------------------

float randomFloat(float min, float max)
{
	return (randomFloat() * (max - min)) + min;
}





// -----------------------------------------------------------------------------------------
// openGLCheckError
// -----------------------------------------------------------------------------------------
// checks if OpenGL has seen an error, and if so aborts
// with the error message - if not, continues silently
// -----------------------------------------------------------------------------------------

void openGLCheckError(void)
{
	GLenum	theError;
	theError = glGetError();
	if (theError == GL_NO_ERROR)
		return;
	fatal ((char *)gluErrorString(theError));
}




// -----------------------------------------------------------------------------------------
// drawText
// -----------------------------------------------------------------------------------------
// draws a string of text at position (x,y) in window space
// -----------------------------------------------------------------------------------------

void drawText(string s, int x, int y) 
{
	int lines = 0;
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, gWindowWidth, 0, gWindowHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glRasterPos2i(x+1, y-1);
	glColor3f(1.0, 1.0, 1.0);

	for(unsigned int i=0; i < s.length(); i++)
	{
		if (s[i] == '\n')
		{
			lines++;
			glRasterPos2i(x+1, y-1-(lines*12));
		}
		else
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}




// -----------------------------------------------------------------------------------------
// calculateFrameRate
// -----------------------------------------------------------------------------------------

float calculateFrameRate(void)
{
		
	static float lastSavedTime = 0.0;
	static long lastSavedFrame = 0;
	static float frameRate = 0.0;
	
	if (gProgramMode == kpmAnimating)
	{
		// update frame rate every half second
		if (gCurrentFrameTime >= lastSavedTime + 0.5)
		{
			frameRate = (float)(gRenderedFrameCounter - lastSavedFrame) / (float)(gCurrentFrameTime - lastSavedTime);
			lastSavedTime = gCurrentFrameTime;
			lastSavedFrame = gRenderedFrameCounter;
		}
	}
	else
	{
		lastSavedTime = 0.0;
		lastSavedFrame = 0;
		frameRate = 0.0;
	}

	return frameRate;

}




// -----------------------------------------------------------------------------------------
// showCopyright
// -----------------------------------------------------------------------------------------
// displays the Music copyright information (required if Music is used in the animation)
// -----------------------------------------------------------------------------------------

void showCopyright(string year, string rightsOwner, string artist, string title)
{
	stringstream s;
	s <<  '"' << title << '"' << " by " << artist << ", (c) " << year << " " << rightsOwner;
	drawText (s.str(), 0, 6);
}





// -----------------------------------------------------------------------------------------
// beginSphericalBillboard
// -----------------------------------------------------------------------------------------
// Begin spherical billboarding - the object will always face the camera.
// Based on code from http://www.lighthouse3d.com/opengl/billboarding
// -----------------------------------------------------------------------------------------

void beginSphericalBillboard(vectr objectLocation)
{
	vectr	lookAt,	objToCamProj, objToCam, upAux;
	float	camX, camY, camZ;
	float	angleCosine;

	lookAt.set (0, 0, 1);
	
	gCamera.getPosition(&camX, &camY, &camZ);

	// objToCamProj is the vector in world coordinates from the local origin to the camera
	// projected in the XZ plane
	objToCamProj.set(camX - objectLocation.data[0], 0, camZ - objectLocation.data[2]);

	// normalize both vectors to get the cosine directly afterwards
	objToCamProj.reduceToUnit();

	// easy fix to determine wether the angle is negative or positive
	// for positive angles upAux will be a vector pointing in the 
	// positive y direction, otherwise upAux will point downwards
	// effectively reversing the rotation.
	upAux = crossProduct(lookAt, objToCamProj);

	// compute the angle
	angleCosine = dotProduct(lookAt, objToCamProj);

	// perform the rotation. The if statement is used for stability reasons
	// if the lookAt and v vectors are too close together then |aux| could
	// be bigger than 1 due to lack of precision
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		glRotatef(acos(angleCosine) * degPerRad, upAux.data[0], upAux.data[1], upAux.data[2]);	
	else if
		(angleCosine <= -0.9999)
			glRotatef(180.0, 0, 1.0, 0);	

	// The second part tilts the object so that it faces the camera

	// objToCam is the vector in world coordinates from the local origin to the camera
	objToCam.set(camX - objectLocation.data[0], camY - objectLocation.data[1], camZ - objectLocation.data[2]);

	// Normalize to get the cosine afterwards
	objToCam.reduceToUnit();

	// Compute the angle between v and v2, i.e. compute the
	// required angle for the lookup vector
	angleCosine = dotProduct(objToCamProj, objToCam);


	// Tilt the object. The test is done to prevent instability when objToCam and objToCamProj have a very small
	// angle between them
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		if (objToCam.data[1] < 0)
			glRotatef(acos(angleCosine) * degPerRad, 1, 0, 0);	
		else
			glRotatef(acos(angleCosine) * degPerRad, -1, 0, 0);	

}





// -----------------------------------------------------------------------------------------
// billboardCylindricalBegin
// -----------------------------------------------------------------------------------------
// Begin cylindrical billboarding - the object will always face the camera.
// Based on code from http://www.lighthouse3d.com/opengl/billboarding
// -----------------------------------------------------------------------------------------

void beginCylindricalBillboard(vectr objectLocation)
{
	vectr	lookAt,	objToCamProj, objToCam, upAux;
	float	camX, camY, camZ;
	float	angleCosine;

	lookAt.set (0, 0, 1);
	
	gCamera.getPosition(&camX, &camY, &camZ);

	// objToCamProj is the vector in world coordinates from the local origin to the camera projected in the XZ plane
	objToCamProj.set(camX - objectLocation.data[0], 0, camZ - objectLocation.data[2]);

	// normalize both vectors to get the cosine directly afterwards
	objToCamProj.reduceToUnit();

	// easy fix to determine wether the angle is negative or positive
	// for positive angles upAux will be a vector pointing in the 
	// positive y direction, otherwise upAux will point downwards
	// effectively reversing the rotation.
	upAux = crossProduct(lookAt, objToCamProj);

	// compute the angle
	angleCosine = dotProduct(lookAt, objToCamProj);

	// perform the rotation. The if statement is used for stability reasons
	// if the lookAt and v vectors are too close together then |aux| could
	// be bigger than 1 due to lack of precision
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		glRotatef(acos(angleCosine) * degPerRad, upAux.data[0], upAux.data[1], upAux.data[2]);
	else if
		(angleCosine <= -0.9999)
			glRotatef(180.0, 0, 1.0, 0);
}





// -----------------------------------------------------------------------------------------
// fatal
// -----------------------------------------------------------------------------------------
// aborts with a console error message
// -----------------------------------------------------------------------------------------

void fatal(string s)
{
	cout << "FATAL: " << s << endl;
#ifdef _WIN32
	Sleep(5000);
#endif
	exit(-1);
}






// -----------------------------------------------------------------------------------------
// sin_d, cos_d, atan_d
// -----------------------------------------------------------------------------------------
// return the sin, cosine or arc-tangent of an angle, when the angle is
// specified in degrees
// -----------------------------------------------------------------------------------------

float sin_d(float f)
{
	return sin(f / degPerRad);
}


float cos_d(float f)
{
	return cos(f / degPerRad);
}


float atan_d(float f)
{
	return atan(f) * degPerRad;
}






// -----------------------------------------------------------------------------------------
// calculateNormal
// -----------------------------------------------------------------------------------------
// takes three vertices, (which must be in anticlockwise order) and calculates a unit normal
// for lighting
// -----------------------------------------------------------------------------------------

void calculateNormal(vectr &out, vectr &a, vectr &b, vectr &c) 
{
	vectr	v1, v2;

	// calculate two vectors from the three points 
	v1 = a - b;
	v2 = b - c;
	
	// take the cross product  
	out = crossProduct(v1, v2);

	// normalize result 
	out.reduceToUnit(); 
}







// -----------------------------------------------------------------------------------------
// interpolate
// -----------------------------------------------------------------------------------------
// this function returns a value that is interpolated between two
// endpoints, based on the current animation time and a start time
// and end time
//
// it takes 4 parameters:
// 		t1              - the start time - if the function is called before 
//						  this time, it returns "p1"
//		t2              - the end time - if the function is called after 
//						  this time, it returns "p2"
//		p1              - one end of the range
//		p2              - the other end of the range
// -----------------------------------------------------------------------------------------

float interpolate(float t1, float t2, float p1, float p2)
{
    float t = gCurrentFrameTime;
    if (t <= t1) return p1;
    if (t >= t2) return p2;
    return (((p2 - p1) / (t2-t1)) * (t-t1)) + p1;
}







// -----------------------------------------------------------------------------------------
// smoothAcceleration
// -----------------------------------------------------------------------------------------
// this function returns a value that is smootly interpolated between two
// endpoints, based on the current animation time, a start time for the
// interpolation sequence, and a "rampup/rampdown" and "steady-state" 
// duration
//
// it takes 5 parameters:
// 		the start time	- if the function is called before this time, it
//						  returns "initialVal"
//		rampDur			- the "rampup" and "rampdown" durations
//		cruiseDur		- the "cruise" duration - this plus 2*rampDur represents
//						  the total duration over which the range is interpolated
//		initialVal		- the value to use at the start of the sequence (this might
//						  be the starting position of a vehicle, for example)
//		finalVal		- the value to use at the end of the sequence (this would be
//						  the ending position of the vehicle)
// -----------------------------------------------------------------------------------------

float smoothAcceleration(float startTime, float rampDur, float cruiseDur, float initialVal, float finalVal)
{
	float duration, acceleration, distance, deltat, rampDistance;
	
	duration = rampDur + cruiseDur + rampDur;
	distance = finalVal - initialVal;
	
	if (gCurrentFrameTime <= startTime) return initialVal;
	if (gCurrentFrameTime > startTime + duration) return finalVal;
	
	acceleration = (2 * distance) / (2*rampDur*rampDur + 2*rampDur*cruiseDur);
	
	// the acceleration phase
	if (gCurrentFrameTime <= startTime + rampDur)
	{
		deltat = gCurrentFrameTime - startTime;
		rampDistance = 0.5 * acceleration * deltat*deltat;
		return rampDistance + initialVal;
	}
	
	// the steady-speed phase
	if (gCurrentFrameTime <= startTime + rampDur + cruiseDur)
	{
		deltat = gCurrentFrameTime - (startTime + rampDur);
		rampDistance = 0.5 * acceleration * rampDur*rampDur;
		return rampDistance + (rampDur * acceleration * deltat) + initialVal;
	}
	
	// the deceleration phase
	deltat = startTime + duration - gCurrentFrameTime;
	rampDistance = 0.5 * acceleration * deltat * deltat;
	return finalVal - rampDistance;
}





// -----------------------------------------------------------------------------------------
// speedUp
// -----------------------------------------------------------------------------------------
// this function returns an accelerating value that is smootly interpolated 
// between two endpoints, based on the current animation time and a 
// rampup duration
//
// it takes 5 parameters:
// 		the start time	- if the function is called before this time, it
//						  returns "initialVal"
//		duration		- the duration over which the value will be interpolated
//		initialVal		- the value to use at the start of the sequence (this might
//						  be the starting position of a vehicle, for example)
//		finalVal		- the value to use at the end of the sequence (this would be
//						  the ending position of the vehicle)
// -----------------------------------------------------------------------------------------

float speedUp(float startTime, float duration, float initialVal, float finalVal)
{
	if (gCurrentFrameTime <= startTime) return initialVal;
	if (gCurrentFrameTime > startTime + duration) return finalVal;
	
	float distance = finalVal - initialVal;
	float acceleration = (2 * distance) / (duration*duration);
	
	float deltat = gCurrentFrameTime - startTime;
	float rampDistance = 0.5 * acceleration * deltat*deltat;
	return rampDistance + initialVal;
}





// -----------------------------------------------------------------------------------------
// objectToCameraDistanceComparison
// -----------------------------------------------------------------------------------------
// used for sorting based on distance from camera
//
// returns TRUE of object A is further away from the camera than object B (the reverse of
// what this sort of function usually does, but we want objects furthest from the camera
// to be sorted to the start of the list)
// -----------------------------------------------------------------------------------------

bool objectToCameraDistanceComparison(object3d *a, object3d *b)
{
	return (a->getDistanceToCamera() > b->getDistanceToCamera());
}





// -----------------------------------------------------------------------------------------
// faceToCameraDistanceComparison
// -----------------------------------------------------------------------------------------
// used for sorting based on distance from camera
//
// returns TRUE of face A is further away from the camera than face B (the reverse of
// what this sort of function usually does, but we want faces furthest from the camera
// to be sorted to the start of the list)
// -----------------------------------------------------------------------------------------

bool faceToCameraDistanceComparison(face a, face b)
{
	return (a.distanceToCamera > b.distanceToCamera);
}





// -----------------------------------------------------------------------------------------
// readShaderFile
// -----------------------------------------------------------------------------------------
// reads the contents of a text file into a C-style array of strings
// these will be used for the shader compiler
// -----------------------------------------------------------------------------------------

char *readShaderFile(string theFilename)
{
	char		*buffer = NULL;
	ifstream 	theFile;
	string		currentLine;


	// get the full path to the shader file
	string fileLocation;
	fileLocation = pathToResourceDirectory() + theFilename;


	// see if we can read the file
	theFile.open(fileLocation.c_str(), ios::in | ios::binary);
	if (!theFile)
	{
		string errMesg = "Unable to open shader file ";
		errMesg.append(theFilename);
		fatal (errMesg);
	}

	theFile.seekg(0, ios::end);					// move to the end of the file to work out how big it is
	unsigned int fileSize = theFile.tellg();
	theFile.seekg(0, ios::beg);					// now back to the start
	
	// swallow the file into the memory buffer
	if (fileSize > 0)
	{
		buffer = (char *)malloc(sizeof(char) * (fileSize+1));
		theFile.read(buffer, fileSize);
	}
	buffer[fileSize] = '\0';

	// all done
	theFile.close();
	return buffer;
}







