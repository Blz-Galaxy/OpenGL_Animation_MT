// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Edited by Evan
//
// composite2.h
//
// This file declares the class for "composite2" object.  


#ifndef _COMPOSITE2_H
#define _COMPOSITE2_H




// -----------------------------------------------------------------------------------------
// application includes
// -----------------------------------------------------------------------------------------
#include "object3d.h"

#include "sweep.h"
#include "extrusion.h"
#include "sphere.h"
#include "torus.h"



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class composite2 : public object3d
{
	public:
		extrusion *face, *body, *pants, *tail, *medal;		// the component parts of the composite2
		extrusion *sword;
		extrusion *arm1, *arm2;
		extrusion *ear1, *ear2;
		extrusion *brow1, *brow2;
		extrusion *hoof1, *hoof2, *hoof3, *hoof4;

		sweep *pants1, *pants2;
		sweep *handle, *shield;
		sweep *sh_handle1, *sh_handle2, *sh_handle3;

		sphere *arm_pivot1, *arm_pivot2, *leg_pivot1, *leg_pivot2, *tail_pivot;
		sphere *spot1, *spot2, *spot3, *spot4, *spot5, *spot6;
		sphere *eye1, *orbit1, *ball1;
		sphere *eye2, *orbit2, *ball2;
		sphere *horn1, *horn2;

		torus *nose1, *nose2, *nose_ring;
		torus *appendant, *sh_ring;

		
		composite2();					// the constructor
					
		virtual void hide(void);
		virtual void show(void);

		virtual void setBodycolor(float,float,float);		// set the color of each part
		virtual void setFacecolor(float,float,float);
		virtual void setFootcolor(float,float,float);
		virtual void setPantscolor(float,float,float);
		
		virtual void hide_equip(void);		// set the decorations
		virtual void show_equip(void);

		virtual void show_sword_only(void);
		virtual void hide_sword(void);

		virtual void show_shield_only(void);
		virtual void hide_shield(void);
};


#endif // _COMPOSITE2_H

