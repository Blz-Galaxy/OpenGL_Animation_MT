// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// snd.h
//
// This is a class used to represent a single sound object, but also contains
// class functions for dealing with a list (STL vector) of sound objects
// (so all sounds can be stopped, started, etc.)
//


#ifndef _snd_H
#define _snd_H



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "platform.h"




// -----------------------------------------------------------------------------------------
// class declaration
// -----------------------------------------------------------------------------------------
class snd
{
	private:
				long	theSound;						// a reference to the loaded sound (may be platform specific)
				bool	playing;						// true = has been started (but may be paused)
				bool	paused;							// true = playing and paused
			
	
	public:
						snd(string theFilename);		// constructor
		virtual			~snd();						// destructor
		
				void	startSound();					// start the sound playing (from the beginning, or the past playpoint)
				void	restartSound();					// restarts the sound (from the beginning)
				void	stopSound();					// stop the sound playing completely
				void	pauseSound();					// stop the sound, but just mark it as "paused"
				void	unpauseSound();					// continue the sound if it was paused
				void	setVolume(float);
				void	setPlayPoint(float);
		
				bool	isPlaying(void)	{ return playing; }
				bool	isPaused(void)	{ return paused; }


		// class functions for dealing with all sounds in the vector
		static	void	stopAllSounds(void);
		static	void	preloadAllSounds(void);
		static	void	pauseAllSounds(void);
		static	void	unpauseAllSounds(void);
		
};



#endif // _sound_H

