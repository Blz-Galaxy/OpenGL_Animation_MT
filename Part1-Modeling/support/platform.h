// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// platform.h
//
// contains platform-specific utility functions

#ifndef _PLATFORM_H
#define _PLATFORM_H


// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"




// -----------------------------------------------------------------------------------------
// general exports
// -----------------------------------------------------------------------------------------
string pathToResourceDirectory(void);

void platformInitialise(void);
void platformWindup(void);

void resetAnimationTimer(void);
float getTimeSinceAnimationStarted(void);
void pauseAnimationTimer(void);
void unpauseAnimationTimer(void);

long platformLoadSoundFile(string theFilename);
void platformUnloadSound(long theSound);
void platformStartSound(long theSound);
void platformStopSound(long theSound);
void platformSoundLevel(long theSound, float theLevel);
void platformSetSoundPlayPoint(long theSound, float theTime);
void platformProcessSound(void);


GLuint platformLoadTextureFile(string theFilename, bool generateMipmaps, int *width, int *height);



#endif // _PLATFORM_H

