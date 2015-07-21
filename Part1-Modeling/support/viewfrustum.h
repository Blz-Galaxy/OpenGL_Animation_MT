// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// viewfrustum.h
//
// This file declares a class for managing viewfrustum objects


#ifndef _viewfrustum_H
#define _viewfrustum_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "vectr.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class viewfrustum
{
	protected:
		double		vfFov;
		double		vfZNear;
		double		vfZFar;


	public:
                    viewfrustum();
                    viewfrustum(double fov, double znear, double zfar);
		virtual     ~viewfrustum();
		
		double		fov(void)
					{ return vfFov; }

		double		zNear(void)
					{ return vfZNear; }

		double		zFar(void)
					{ return vfZFar; }
					
		void		setFov(double d)
					{ vfFov = d; }

		void		setzNear(double d)
					{ vfZNear = d; }

		void		setzFar(double d)
					{ vfZFar = d; }

		void        tellGL(void);			// send viewfrustum data to OpenGL

};


#endif // _viewfrustum_H


