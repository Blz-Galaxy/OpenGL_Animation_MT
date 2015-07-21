// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// fragmentshader.cpp
//
// This file defines a class for OpenGL fragment shaders




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "fragmentshader.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// default fragmentshader constructor
// -----------------------------------------------------------------------------------------

fragmentshader::fragmentshader(string theFilename)
{
	if (GLEW_VERSION_2_0)
	{
		shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		shaderText = readShaderFile(theFilename);
		const GLchar *program = shaderText;
		glShaderSource(shaderID, 1, &program, NULL);
		glCompileShader(shaderID);
		
		GLint retval;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &retval);
		if (retval == GL_FALSE)
		{
			cout << "fragment shader " << theFilename << " compile failed\n";
		}
			
		GLint infologLength = 0;
		GLint charsWritten  = 0;
		char *infoLog;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(shaderID, infologLength, &charsWritten, infoLog);
			printf("fragment shader info log: %s\n",infoLog);
			free(infoLog);
		}
	}
}





// -----------------------------------------------------------------------------------------
// fragmentshader destructor
// -----------------------------------------------------------------------------------------

fragmentshader::~fragmentshader()
{
	if (shaderID) glDeleteShader(shaderID);
}







