// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Edited by Evan
//
// composite1.h
//
// This file declares the class for "composite1" object.  


#ifndef _COMPOSITE1_H
#define _COMPOSITE1_H



// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"

#include "sweep.h"
#include "extrusion.h"
#include "cube.h"
#include "treasurebox.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class composite1 : public object3d
{
	public:
		extrusion *h_frame1, *h_frame2;				// the component parts of the composite1
		extrusion *v_frame1, *v_frame2, *v_frame3, *v_frame4;
		extrusion *lid, *lid_frame1, *lid_frame2, *lid_frame3;

		sweep *milk, *diamond[10];

		cube *pivot;

		treasurebox *outface, *insideface;


		composite1();					// the constructor

		virtual void hide(void);
		virtual void show(void);
		virtual void openbox(float);
		virtual void closebox(float);
};


#endif // _COMPOSITE1_H

