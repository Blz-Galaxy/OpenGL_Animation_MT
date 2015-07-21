// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// skybox.cpp
//
// This file defines the class for a "skybox" object. A skybox consists of 6 images loaded
// from 6 JPG files.  The 6 image files share a common name, the base name being passed
// into the constructor.
//
// Because many video cards seem to have trouble rendering seamless skyboxes, we resort
// to a trick here.  We pass texture coordinates for the skybox faces such that the first
// and last pixels are clipped.  This tends to hide the seams, but means that two texels
// are lost at each edge of the box.  With suitably large textures, this shouldn't be
// noticeable (especially if the images don't have harsh transitions near the edges). 



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "skybox.h"
#include "utility.h"
#include "camera.h"



// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern	unsigned long	gPolygonCount;			// counts how many polygons get displayed each frame
extern	cameraClass		gCamera;				// the camera




// -----------------------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------------------
// the parameter passed in is:
// basename		- the name of the file (a digit and ".jpg" will be appended for each of the 6 files)

skybox::skybox(string filenameBase)
{
	setName("skybox_" + filenameBase);

	for (int i=0; i<6; i++)
	{
		// make the texture name from the base name
		stringstream filename;
		filename << filenameBase << i+1 << ".jpg";
		
		// create a new texture for this face, specify continuous tone and no MIPMAPS
		skytex[i] = new texture(filename.str(), kContinuousTone, kNoMipmaps);
	}
}




// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------
skybox::~skybox()
{
	for (int i=0; i<6; i++)
		delete skytex[i];
}




// -----------------------------------------------------------------------------------------
// private draw method
// -----------------------------------------------------------------------------------------
// we try to draw the skybox as optimally as possible
void skybox::privateDraw(bool drawMode)
{
	const GLshort filterMode = GL_LINEAR;
	
	// bring the coordinates in by one pixel to create seamless textures
	float min = 1.0 / skytex[0]->width();
	float max = 1.0 - (1.0 / skytex[0]->width());
	
	// set the correct drawing state
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gViewFrustum->tellGL();
	glMatrixMode(GL_MODELVIEW);
	gCamera.tellGLCentered();
	
	
 	// first draw the left face
	glBindTexture(GL_TEXTURE_2D, skytex[0]->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(min, min);	glVertex3f(-1.0, -1.0,   1.0);		// bot left of texture, bot front of quad
	glTexCoord2f(max, min);	glVertex3f(-1.0, -1.0,  -1.0);		// bot right of texture, bot back of quad
	glTexCoord2f(max, max);	glVertex3f(-1.0,  1.0,  -1.0);		// top right of texture, top back of quad
	glTexCoord2f(min, max);	glVertex3f(-1.0,  1.0,   1.0);		// top left of texture, top front of quad
	glEnd();
	
	// next draw the back face
	glBindTexture(GL_TEXTURE_2D, skytex[1]->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(min, min);	glVertex3f(-1.0, -1.0,  -1.0);		// bot left of texture and quad
	glTexCoord2f(max, min);	glVertex3f( 1.0, -1.0,  -1.0);		// bot right of texture and quad
	glTexCoord2f(max, max);	glVertex3f( 1.0,  1.0,  -1.0);		// top right of texture and quad
	glTexCoord2f(min, max);	glVertex3f(-1.0,  1.0,  -1.0);		// top left of texture and quad
	glEnd();
	
	// then the right face
	glBindTexture(GL_TEXTURE_2D, skytex[2]->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(min, min);	glVertex3f( 1.0, -1.0,  -1.0);		// bot left of texture, bot back of quad
	glTexCoord2f(max, min);	glVertex3f( 1.0, -1.0,   1.0);		// bot right of texture, bot front of quad
	glTexCoord2f(max, max);	glVertex3f( 1.0,  1.0,   1.0);		// top right of texture, top front of quad
	glTexCoord2f(min, max);	glVertex3f( 1.0,  1.0,  -1.0);		// top left of texture, top back of quad
	glEnd();
	
	// then the front face
	glBindTexture(GL_TEXTURE_2D, skytex[3]->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(min, min);	glVertex3f( 1.0, -1.0,   1.0);		// bot left of texture, bot right of quad (backfacing)
	glTexCoord2f(max, min);	glVertex3f(-1.0, -1.0,   1.0);		// bot right of texture, bot left of quad (backfacing)
	glTexCoord2f(max, max);	glVertex3f(-1.0,  1.0,   1.0);		// top right of texture, top left of quad (backfacing)
	glTexCoord2f(min, max);	glVertex3f( 1.0,  1.0,   1.0);		// top left of texture, top right of quad (backfacing)
	glEnd();
		
	// then the top face
	glBindTexture(GL_TEXTURE_2D, skytex[4]->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(min, min);	glVertex3f(-1.0,  1.0,   1.0);		// bot left of texture, front left of quad
	glTexCoord2f(max, min);	glVertex3f(-1.0,  1.0,  -1.0);		// bot right of texture, back left of quad
	glTexCoord2f(max, max);	glVertex3f( 1.0,  1.0,  -1.0);		// top right of texture, back right of quad
	glTexCoord2f(min, max);	glVertex3f( 1.0,  1.0,   1.0);		// top left of texture, front right of quad
	glEnd();

	// then the bottom face
	glBindTexture(GL_TEXTURE_2D, skytex[5]->id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBegin(GL_QUADS);
	glTexCoord2f(min, max);	glVertex3f(-1.0, -1.0,   1.0);		// top left of texture, front left of quad
	glTexCoord2f(min, min);	glVertex3f( 1.0, -1.0,   1.0);		// bot left of texture, front right of quad
	glTexCoord2f(max, min);	glVertex3f( 1.0, -1.0,  -1.0);		// bot right of texture, back right of quad
	glTexCoord2f(max, max);	glVertex3f(-1.0, -1.0,  -1.0);		// top right of texture, back left of quad
	glEnd();


	// restore the settings we altered earlier
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	// the skybox isn't drawn using the normal mechanism, so update the global polygon count manually
	gPolygonCount += 6;

}




