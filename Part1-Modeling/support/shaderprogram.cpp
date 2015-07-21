// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// shaderprogram.cpp
//
// This file defines a class for OpenGL shader programs




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "shaderprogram.h"




// -----------------------------------------------------------------------------------------
// default shaderprogram constructor
// -----------------------------------------------------------------------------------------

shaderprogram::shaderprogram()
{
	if (GLEW_VERSION_2_0) programID = glCreateProgram();
	else programID = 0;
}





// -----------------------------------------------------------------------------------------
// shaderprogram destructor
// -----------------------------------------------------------------------------------------

shaderprogram::~shaderprogram()
{
	if (programID)
		glDeleteProgram(programID);
}





// -----------------------------------------------------------------------------------------
// attach a shader to this program
// -----------------------------------------------------------------------------------------

void shaderprogram::attachShader(vertexshader *v)
{
	if (programID)
		glAttachShader(programID, v->shaderID);
}

void shaderprogram::attachShader(fragmentshader *f)
{
	if (programID)
		glAttachShader(programID, f->shaderID);
}



// -----------------------------------------------------------------------------------------
// link the shader program
// -----------------------------------------------------------------------------------------

void shaderprogram::linkShaderProgram()
{
	if (programID)
	{
		glLinkProgram(programID);
		
		GLint retval;
		glGetProgramiv(programID, GL_LINK_STATUS, &retval);
		if (retval == GL_FALSE)
			cout << "shader program link failed\n";

		GLint infologLength = 0;
		GLint charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(programID, infologLength, &charsWritten, infoLog);
			printf("shader program link info log: %s\n",infoLog);
			free(infoLog);
		}
	}
}



// -----------------------------------------------------------------------------------------
// disable the shader program - CLASS METHOD
// -----------------------------------------------------------------------------------------

void shaderprogram::deactivate()
{
	if (GLEW_VERSION_2_0) glUseProgram(0);
}






