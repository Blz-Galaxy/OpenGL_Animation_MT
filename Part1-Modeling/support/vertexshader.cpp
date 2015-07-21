// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// vertexshader.cpp
//
// This file defines a class for OpenGL vertex shaders




// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "vertexshader.h"
#include "utility.h"




// -----------------------------------------------------------------------------------------
// default vertexshader constructor
// -----------------------------------------------------------------------------------------

vertexshader::vertexshader(string theFilename)
{
	if (GLEW_VERSION_2_0)
	{
		shaderID = glCreateShader(GL_VERTEX_SHADER);	// creates a single vertex shader
		shaderText = readShaderFile(theFilename);		// user-function to get the shader source to memory
		const GLchar *program = shaderText;				// compilers complain almost as much as lecturers...
		glShaderSource(shaderID, 1, &program, NULL);	// associate the shader source with the shader
		glCompileShader(shaderID);						// compile the shader source

		GLint retval;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &retval);
		if (retval == GL_FALSE)
		{
			cout << "vertex shader " << theFilename << " compile failed\n";
		}

		GLint infologLength = 0;
		GLint charsWritten  = 0;
		char *infoLog;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(shaderID, infologLength, &charsWritten, infoLog);
			printf("vertex shader info log: %s\n",infoLog);
			free(infoLog);
		}
	}
}





// -----------------------------------------------------------------------------------------
// vertexshader destructor
// -----------------------------------------------------------------------------------------

vertexshader::~vertexshader()
{
	if (shaderID) glDeleteShader(shaderID);
}







