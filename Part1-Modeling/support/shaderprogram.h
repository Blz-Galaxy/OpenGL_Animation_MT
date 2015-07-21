// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// shaderprogram.h
//
// This file declares a class for OpenGL shaders programs
//


#ifndef _SHADERPROGRAM_H
#define _SHADERPROGRAM_H





// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "vertexshader.h"
#include "fragmentshader.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class shaderprogram
{
	public:
		GLuint			programID;						// OpenGL gives this to us (like a texture ID)


						shaderprogram();				// default constructor
		virtual			~shaderprogram();				// destructor

		void			attachShader(vertexshader *v);
		void			attachShader(fragmentshader *f);
		void			linkShaderProgram();
		virtual void	activate() = 0;
		static void		deactivate();
	
};


#endif // _SHADERPROGRAM_H

