// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// fog.h
//
// This file declares a class for managing fog objects


#ifndef _fog_H
#define _fog_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "vectr.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class fog
{
	protected:
        float       fogStart;
        float       fogEnd;
        vectr       fogColour;


	public:
                    fog();
                    fog(float start, float end, vectr &colour);
		virtual     ~fog();

		void        tellGL(void);			// send fog data to OpenGL

};


#endif // _fog_H


