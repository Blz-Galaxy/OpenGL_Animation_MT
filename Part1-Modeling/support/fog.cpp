// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// fog.cpp
//
// This file defines a class for managing fog objects
//
// declare a fog object with:
//
// fog f = new fog(startDistance, endDistance, colour);
//
// note that only the GL_LINEAR fog equation is support by this class



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "fog.h"




// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------
// if a fog is constructed with no parameters, default all values
// -----------------------------------------------------------------------------------------

fog::fog()
{
    fogStart = 10.0;
    fogEnd = 400.0;
    fogColour.set(0.5, 0.5, 0.5, 1.0);
}




// -----------------------------------------------------------------------------------------
// fog constructor when all parameters specified
// -----------------------------------------------------------------------------------------
// if a fog is constructed with no parameters, default all values
// -----------------------------------------------------------------------------------------

fog::fog(float start, float end, vectr &colour)
{
    fogStart = start;
    fogEnd = end;
    fogColour.set(colour);
}




// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------

fog::~fog()
{
}




// -----------------------------------------------------------------------------------------
// tellGL
// -----------------------------------------------------------------------------------------
// send the fog details to OpenGL
// -----------------------------------------------------------------------------------------

void fog::tellGL()
{
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColour.data);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, fogStart);
    glFogf(GL_FOG_END, fogEnd);
}					





