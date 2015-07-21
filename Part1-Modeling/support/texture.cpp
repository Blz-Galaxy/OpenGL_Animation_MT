// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// texture.cpp
//
// This file defines a class for managing textures
//
// declare a texture with:
//
// texture t = new texture(filename); // for kDiscreteTone, kNoMipMaps
// texture t = new texture(filename, toneHint, mipmaphint);



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "constants.h"
#include "texture.h"
#include "platform.h"




// -----------------------------------------------------------------------------------------
// actual texture loader (private)
// -----------------------------------------------------------------------------------------
// this function "loads" the texture data from the specified texture file (it actually 
// passes the texture filename to the loadTextureFile function, which is in the 
// platform-dependent file (macos.cpp/windows.cpp)
// -----------------------------------------------------------------------------------------

void texture::load(string theFilename, bool tHint, bool bMipmaps)
{
	textureID = platformLoadTextureFile(theFilename, bMipmaps, &loadedWidth, &loadedHeight);
	toneHint = tHint;
	mipmapsGenerated = bMipmaps;
}





// -----------------------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------------------
// if a texture is constructed with no parameters, default all values
// -----------------------------------------------------------------------------------------

texture::texture()
{
	textureID = 0L;
	toneHint = false;
	mipmapsGenerated = false;
}





// -----------------------------------------------------------------------------------------
// constructor when just a filename is specified
// -----------------------------------------------------------------------------------------
// if a texture is constructed with just a filename, set the tonehint to false ("discrete-
// tone" image) and don't ask for mipmaps to be generated
// -----------------------------------------------------------------------------------------

texture::texture(string theFilename)
{
	load(theFilename, false, false);
}




// -----------------------------------------------------------------------------------------
// constructor when all parameters specified
// -----------------------------------------------------------------------------------------
// when a texture is constructed with all the parameters specified, pass them all directly
// to the load function
// -----------------------------------------------------------------------------------------

texture::texture(string theFilename, bool tHint, bool bMipmaps)
{
	load(theFilename, tHint, bMipmaps);
}



// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------

texture::~texture()
{
}




// -----------------------------------------------------------------------------------------
// return the most appropriate filter for magnification
// -----------------------------------------------------------------------------------------
// for a continuous-tone image, we want GL_LINEAR (linear interpolation), and for
// a discrete-tone image, we want GL_NEAREST (so the texture doesn't appear out of focus)
// -----------------------------------------------------------------------------------------

GLuint texture::bestMagFilter()
{
	if (toneHint)
		return GL_LINEAR;
	else
		return GL_NEAREST;
}




// -----------------------------------------------------------------------------------------
// return the most appropriate filter for minification
// -----------------------------------------------------------------------------------------
// for a continuous-tone image, we want GL_LINEAR_MIPMAP_LINEAR (linear interpolation on
// the nearest mipmap, if mipmaps were generated), or GL_LINEAR (linear interpolation) if
// they weren't
// for discrete-tone images, we want GL_NEAREST (so the texture doesn't appear out of focus)
// -----------------------------------------------------------------------------------------

GLuint texture::bestMinFilter()
{
	if (toneHint && mipmapsGenerated)
		return GL_LINEAR_MIPMAP_LINEAR;
	if (toneHint)
		return GL_LINEAR;

	return GL_NEAREST;
}





