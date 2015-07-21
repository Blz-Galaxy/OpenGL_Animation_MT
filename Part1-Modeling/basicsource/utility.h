// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// utility.h
//
// contains general utility functions for the program

#ifndef _UTILITY_H
#define _UTILITY_H


#include "vectr.h"
#include "object3d.h"




// random number generation (for particle systems)
float		randomFloat(void);
float		randomFloat(float min, float max);



// OpenGL
void		openGLCheckError(void);
void 		drawText(string s, int x, int y);
float		calculateFrameRate();
void		showCopyright(string year, string rightsOwner, string artist, string title);
void		beginSphericalBillboard(vectr objectLocation);
void		beginCylindricalBillboard(vectr objectLocation);



// debugging
void		fatal(string s);


// trig
float		sin_d(float f);
float		cos_d(float f);
float		atan_d(float f);


// misc
void		calculateNormal(vectr &out, vectr &a, vectr &b, vectr &c);
float		interpolate(float t1, float t2, float p1, float p2);
float		smoothAcceleration(float start, float rampDur, float cruiseDur, float initial, float final);
float		speedUp(float start, float rampDur, float initial, float final);
char		*readShaderFile(string theFilename);


// distance sorting
bool		objectToCameraDistanceComparison(object3d *a, object3d *b);
bool		faceToCameraDistanceComparison(face a, face b);



#endif // _UTILITY_H

