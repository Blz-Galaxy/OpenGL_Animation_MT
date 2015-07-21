// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// snd.cpp
//
// This is a class used to represent a single sound object, but also defines
// class functions for dealing with a list (STL vector) of sound objects
// (so all sounds can be stopped, started, etc.)
// it also declares the global sound vector



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "snd.h"




// -----------------------------------------------------------------------------------------
// globals
// -----------------------------------------------------------------------------------------
// define a global vector of sound object pointers, and an iterator for them
// so we can walk the list of sounds 
// -----------------------------------------------------------------------------------------
vector<snd*> soundVector;
typedef vector<snd*>::iterator soundVectorIterator;





// -----------------------------------------------------------------------------------------
// snd constructor
// -----------------------------------------------------------------------------------------
// loads the sound file (using a platform-specific function), and then pushes the
// sound onto the end of the sound vector
// sounds don't start playing when they're constructed - they must be "started"
// -----------------------------------------------------------------------------------------

snd::snd(string theFilename)
{
	theSound = platformLoadSoundFile(theFilename);
	playing = false;
	paused = false;
	if (theSound) soundVector.push_back(this);
}




// -----------------------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------------------
// ensures this sound is removed from the sound vector, and calls the platform-specific
// sound unload function
// -----------------------------------------------------------------------------------------

snd::~snd()
{
	soundVectorIterator svi;
	for (svi = soundVector.begin(); svi != soundVector.end(); svi++)
		if (*svi == this)
			soundVector.erase(svi);
	platformUnloadSound(theSound);
}




// -----------------------------------------------------------------------------------------
// startSound()
// -----------------------------------------------------------------------------------------
// starts this sound playing
// -----------------------------------------------------------------------------------------

void snd::startSound()
{
	if (!playing)
	{
		platformStartSound(theSound);
		playing = TRUE;
		paused = FALSE;
	}
}




// -----------------------------------------------------------------------------------------
// restartSound()
// -----------------------------------------------------------------------------------------
// starts this sound playing again from the beginning
// -----------------------------------------------------------------------------------------

void snd::restartSound()
{
	stopSound();
	setPlayPoint(0.0);
	startSound();
}




// -----------------------------------------------------------------------------------------
// stopSound()
// -----------------------------------------------------------------------------------------

void snd::stopSound()
{
	if (playing)
	{
		platformStopSound(theSound);
		playing = FALSE;
		paused = FALSE;
	}
}




// -----------------------------------------------------------------------------------------
// pauseSound()
// -----------------------------------------------------------------------------------------

void snd::pauseSound()
{
	if (playing && (!paused))
	{
		platformStopSound(theSound);
		paused = TRUE;
	}
}




// -----------------------------------------------------------------------------------------
// unpauseSound()
// -----------------------------------------------------------------------------------------

void snd::unpauseSound()
{
	if (playing && paused)
	{
		platformStartSound(theSound);
		paused = FALSE;
	}
}




// -----------------------------------------------------------------------------------------
// setVolume()
// -----------------------------------------------------------------------------------------
// sets the volume of the sound
// the parameter is a float that represents the volume - range 0-1
// -----------------------------------------------------------------------------------------

void snd::setVolume(float volume)
{
	platformSoundLevel(theSound, volume);
}




// -----------------------------------------------------------------------------------------
// setPlayPoint()
// -----------------------------------------------------------------------------------------
// sets the position of the playback head
// the parameter is a float that represents the time value in seconds
// -----------------------------------------------------------------------------------------

void snd::setPlayPoint(float time)
{
	platformSetSoundPlayPoint(theSound, time);
}




// -----------------------------------------------------------------------------------------
// stopAllSounds() - CLASS METHOD
// -----------------------------------------------------------------------------------------
// walks the list of all sounds, and stops any that are playing
// -----------------------------------------------------------------------------------------

void snd::stopAllSounds(void)
{
	soundVectorIterator svi;
	for (svi = soundVector.begin(); svi != soundVector.end(); svi++)
		(*svi)->stopSound();
}




// -----------------------------------------------------------------------------------------
// pauseAllSounds() - CLASS METHOD
// -----------------------------------------------------------------------------------------
// walks the list of all sounds, and pauses any that are playing
// nonplaying sounds are not affected by this
// -----------------------------------------------------------------------------------------

void snd::pauseAllSounds(void)
{
	soundVectorIterator svi;
	for (svi = soundVector.begin(); svi != soundVector.end(); svi++)
		(*svi)->pauseSound();
}




// -----------------------------------------------------------------------------------------
// unpauseAllSounds() - CLASS METHOD
// -----------------------------------------------------------------------------------------
// walks the list of all sounds, and unpauses any that are paused
// nonplaying sounds are not affected by this
// -----------------------------------------------------------------------------------------

void snd::unpauseAllSounds(void)
{
	soundVectorIterator svi;
	for (svi = soundVector.begin(); svi != soundVector.end(); svi++)
		(*svi)->unpauseSound();
}




// -----------------------------------------------------------------------------------------
// preloadAllSounds() - CLASS METHOD
// -----------------------------------------------------------------------------------------
// walks the list of all sounds, stops them playing if necessary, rewinds then, starts
// and immediately stops them again (used to get all sounds loaded before we start animating)
// called when the animation is (re)started
// -----------------------------------------------------------------------------------------

void snd::preloadAllSounds(void)
{
	soundVectorIterator svi;
	for (svi = soundVector.begin(); svi != soundVector.end(); svi++)
	{
		(*svi)->setVolume(0.0);			// don't want any "clicks"
		(*svi)->startSound();			// start playing...
		(*svi)->stopSound();			// ... but immediately stop
		(*svi)->setPlayPoint(0.0);		// rewind
		(*svi)->setVolume(1.0);			// back to normal volume
	}
}




