// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// skybox.h
//
// This file defines the class for a "skybox" object. A skybox consists of 6 images loaded
// from 6 JPG files.  The 6 image files share a common name, the base name being passed
// into the constructor.


#ifndef _SKYBOX_H
#define _SKYBOX_H




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"
#include "viewfrustum.h"



// -----------------------------------------------------------------------------------------
// externals
// -----------------------------------------------------------------------------------------
extern viewfrustum	*gViewFrustum;



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class skybox : public object3d
{
	private:
			texture		*skytex[6];							// array of texture objects - l/b/r/f/u/d
						
	public:
						skybox(string filenameBase);		// constructor
			virtual		~skybox();							// destructor
				
			virtual void privateDraw(bool drawMode);
				
			virtual	skybox*	clone()	const					// clone myself
							{ return new skybox(*this); }		

};



#endif // _SKYBOX_H


