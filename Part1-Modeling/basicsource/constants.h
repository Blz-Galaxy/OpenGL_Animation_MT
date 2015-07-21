// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// constants.h
//
// this file defines application-wide constants


#ifndef _CONSTANTS_H
#define _CONSTANTS_H


// window-size constants
const int kWindowWidth = 720;
const int kWindowHeight = 576;


// the frame rate is normally limited - this time is in seconds, and frames are
// not normally drawn more frequently than this (can be turned off)
const float kTimeBetweenAnimates = 0.01;			// we cauculate at 100 fps
const float kTimeBetweenRedraws = 0.016666;			// we draw at 60 fps (Macintosh only, not limited on Windows)


// constants for transparent drawing
const bool kTransparentMode = true;
const bool kOpaqueMode = false;


// constants for texture loading
const bool kMipmaps = true;
const bool kNoMipmaps = false;
const bool kContinuousTone = true;
const bool kDiscreteTone = false;


// constants for billboard types
const bool kCylindricalBillboarding = true;
const bool kSphericalBillboarding = false;


// constants for light types
const bool kPositionalLight = true;
const bool kDirectionalLight = false;


// maximum texture size to use (any textures larger than this size are downscaled)
const int gMaxTextureSize = 512;



enum ProgramMode
{
	kpmAtStart, kpmAnimating, kpmMakingMovie, kpmPaused, kpmFinished 
};


#define SHADER_SUPPORT

#endif // _CONSTANTS_H



