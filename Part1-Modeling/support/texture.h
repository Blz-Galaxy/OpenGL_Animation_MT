// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// texture.h
//
// This file declares a class for managing textures


#ifndef _TEXTURE_H
#define _TEXTURE_H



// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class texture
{
	protected:
		GLuint		textureID;								// the OpenGL texture name
		int			loadedWidth;							// width and height of the loaded version
		int			loadedHeight;							// (may have been scaled during load)
		bool		mipmapsGenerated;						// true if mipmaps were requested
		bool		toneHint;								// true for continuous-tone images (like photos)
															// false for discrete-tone images (where sharp edges are required)
		
		void		load(string theFilename, bool, bool);	// actual loader


	public:
					texture();
					texture(string theFilename);
					texture(string theFilename, bool, bool);
		virtual		~texture();

		GLuint		id(void)								// return the OpenGL texture ID that identifies this texture
					{ return textureID; }
		
		GLuint		bestMagFilter(void);					// return the best magnification filter
		GLuint		bestMinFilter(void);					// return the best minification filter
		
		int			width(void)								// return the width and height of the
					{ return loadedWidth; }					// loaded texture
		int			height(void)
					{ return loadedHeight; }

};


#endif // _TEXTURE_H


