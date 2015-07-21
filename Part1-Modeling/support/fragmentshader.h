// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// fragmentshader.h
//
// This file declares a class for OpenGL fragment shaders
//
// A fragment shader can't be used by itself - it must be attached to a shader program.


#ifndef _FRAGMENTSHADER_H
#define _FRAGMENTSHADER_H





// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class fragmentshader
{
	public:
		GLuint			shaderID;								// OpenGL gives this to us (like a texture ID)
		GLchar			*shaderText;							// pointer to text shader source from text file
		


						fragmentshader(string theFilename);		// default constructor
		virtual			~fragmentshader();						// destructor


};


#endif // _FRAGMENTSHADER_H

