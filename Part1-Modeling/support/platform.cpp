// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// platform.cpp
//
// contains MacOS and Windows implementations of platform-specific functions



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "platform.h"
#include "constants.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// local platform-specific "globals"
// -----------------------------------------------------------------------------------------
#ifdef __APPLE__ // Mac-specific includes
unsigned long		tStartTime;				// the time that the animation was started
unsigned long		tPauseStarted;			// the time the pause started (for the current pause only)
unsigned long		tPausedTime;			// the total time that the animation was paused
UnsignedWide		theTime;
#endif

#ifdef _WIN32
time_t				tStartTime;				// starting time of clock
time_t				tPauseStarted;			// the time the pause started (for the current pause only)
time_t				tPausedTime;			// the total time that the animation was paused
#endif



// These next values are all filled in by the user from the standard compression
// dialog box via SetCompression(), and then used in the RenderMovie function
// when the movie is being made.  Appropriate defaults are used here...
#ifdef __APPLE__
CodecType			gCodecType = 'avc1';						// H.264
CodecQ				gCodecQuality = codecHighQuality;			// high spatial quality
CodecQ				gCodecTemporalQuality = 0;					// default temporal quality
int					gCodecDepth = 32;							// bit depth
long				gMovieFrameRate = 25;						// desired framte rate (forced, actually)
long				gKeyFrameRate = 25;							// key frame rate
CodecComponent		gCodecID = anyCodec;
#endif
	




// -----------------------------------------------------------------------------------------
// external functions
// -----------------------------------------------------------------------------------------
extern void animateScene(float time, long frame);
extern void initScene();
extern void drawScene();




// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern ProgramMode gProgramMode;
extern long	gAnimatedFrameCounter;	
extern long	gRenderedFrameCounter;	
extern float gCurrentFrameTime;	




// -----------------------------------------------------------------------------------------
// GetTextureDimFromImageDim()
// NOT EXPORTED - INTERNAL USE ONLY
// -----------------------------------------------------------------------------------------
// returns the nearest power of 2 to the size passed in (but not greater than gMaxTextureSize)
// this is used to determine the optimum size for a texture
// -----------------------------------------------------------------------------------------

long GetTextureDimFromImageDim (long imageDimension)  
{
	// find the power of 2 greater
	long i = 0, imageTemp = imageDimension;
	
	while (imageTemp >>= 1)						// while the dimension still has bits of data shift right (losing a bit at a time)
		i++;									// count shifts (i.e., powers of two)

	long texDim = 1;
	texDim = texDim << i;						// shift our one bit representation left the like amount (i.e., 2 ^ i)
	
	// if the texture us too big...
	if (texDim >= gMaxTextureSize)
		return gMaxTextureSize;					// then return the maximum texture size
		
	// are we closer to the greater power of 2 or the lesser power of 2?
	else if (((texDim << 1) - imageDimension) <  (imageDimension - texDim))
		return (texDim << 1);					// closer to the greater power of 2, so return this
		
	return texDim;								// closer to the smaller power of 2, so return this
	
}




// -------------------------------------------------------------------------------------
// pathToResourceDirectory
// -------------------------------------------------------------------------------------
// returns the path to the resources directory as a C++ string type - this is
// where textures, music and other data files needed at runtime are stored
// for MacOS X, this is the Resources folder in the application bundle
// for Windows, this is the "resources" folder alongside the executable
// -------------------------------------------------------------------------------------

string pathToResourceDirectory()
{
#ifdef __APPLE__
	char path[MAXPATHLEN];
	string tempString;
	CFBundleRef bundle;
	CFURLRef resDirURL;

	path[0] = '\0';
	
	bundle = CFBundleGetMainBundle();
	if (!bundle) return string ("");

	resDirURL = CFBundleCopyResourcesDirectoryURL(bundle);
	if (!resDirURL) return string ("");

	CFURLGetFileSystemRepresentation(resDirURL, TRUE, (UInt8 *)path, MAXPATHLEN);
	CFRelease(resDirURL);
	
	// put a trailing slash on the path
	tempString = string(path);
	tempString.append("/");
	return tempString;
#endif

#ifdef _WIN32
	return string("resources/");
#endif
}





// -----------------------------------------------------------------------------------------
// platformInitialiase()
// -----------------------------------------------------------------------------------------
// performs any platform-specific initialisation when the program first starts up
// used to initialise QuickTime for music playback
// -----------------------------------------------------------------------------------------

void platformInitialise(void)
{
#ifdef QuickTimeInstalled
#ifdef _WIN32
	InitializeQTML(0);
#endif
	EnterMovies();
#endif // QuickTimeInstalled
} 





// -----------------------------------------------------------------------------------------
// platformWindup()
// -----------------------------------------------------------------------------------------
// performs any platform-specific deinitialisation when the program terminates
// used to shut down QuickTime
// -----------------------------------------------------------------------------------------
void platformWindup(void)
{
#ifdef QuickTimeInstalled
	ExitMovies();
#ifdef _WIN32
	TerminateQTML();
#endif
#endif // QuickTimeInstalled
} 





// -----------------------------------------------------------------------------------------
// resetAnimationTimer()
// -----------------------------------------------------------------------------------------
// sets the animation timer to zero seconds
// Microseconds is a MacOS function that returns the current time in microseconds
// as an unsigned wide value (an unsigned wide is a struct, that contains two longs)
//
// To reset the animation timer, we grab the low long, which is all that's really
// important to us) and save this in the "tStartTime" variable.  All timing functions
// are then based on the difference between this initial snagged value, and the current
// value returned by Microseconds.
// -----------------------------------------------------------------------------------------

void resetAnimationTimer(void)
{
#ifdef __APPLE__
	Microseconds (&theTime);			// get the current "time"
	tStartTime = theTime.lo;			// snag the low long
	tPausedTime = 0;					// so far, there have been no pauses
#endif

#ifdef _WIN32
	tStartTime = clock();				// snag the time
	tPausedTime = 0;					// so far, there have been no pauses
#endif
} 




// -----------------------------------------------------------------------------------------
// pauseAnimationTimer()
// -----------------------------------------------------------------------------------------
// to "pause" the timer, we record the current time in microseconds (in tPauseStarted)
// -----------------------------------------------------------------------------------------

void pauseAnimationTimer(void)
{
#ifdef __APPLE__
	UnsignedWide theTime;
	Microseconds (&theTime);
	tPauseStarted = theTime.lo;
#endif

#ifdef _WIN32
	tPauseStarted = clock();
#endif
} 




// -----------------------------------------------------------------------------------------
// unpauseAnimationTimer()
// -----------------------------------------------------------------------------------------
// to "unpause" the timer, we get the time in microseconds again, and subtract
// this from the time at which we started the pause - the result is the time
// that we were paused - this value is added to the overall paused amount
// -----------------------------------------------------------------------------------------

void unpauseAnimationTimer(void)
{
#ifdef __APPLE__
	UnsignedWide theTime;
	Microseconds (&theTime);
	unsigned long thisPause = (theTime.lo - tPauseStarted);
	tPausedTime += thisPause;
#endif

#ifdef _WIN32
	time_t thisPause = (clock() - tPauseStarted);
	tPausedTime += thisPause;
#endif
} 




// -----------------------------------------------------------------------------------------
// getTimeSinceAnimationStarted()
// -----------------------------------------------------------------------------------------
// Returns the time in seconds since the animation was started.
// We subtract the total paused time from this to account for paused time.
// Note that this final value is divided  by one million (Mac) or one thousand (Windows)
// so that the returned time is a float representing seconds.
// -----------------------------------------------------------------------------------------

float getTimeSinceAnimationStarted(void)
{
	static float retValue;
	
#ifdef __APPLE__
	if (gProgramMode == kpmMakingMovie)						// if doing QuickTime Movie output, return a fixed value
		return (gRenderedFrameCounter / (float)gMovieFrameRate);	// based on the frame counter and frame rate
#endif
		
	if (gProgramMode == kpmFinished)					// if we're done, return the last animated timer
		return retValue;								// value (so the timer doesn't increment on a redraw)
		
#ifdef __APPLE__
	UnsignedWide theTime;
	Microseconds (&theTime);
	retValue = (theTime.lo - tStartTime - tPausedTime) / 1000000.0;
#endif

#ifdef _WIN32
	retValue = (clock() - tStartTime - tPausedTime) / 1000.0;
#endif

	return retValue;
} 





// -----------------------------------------------------------------------------------------
// platformLoadSoundFile()
// -----------------------------------------------------------------------------------------
// uses QuickTime to load a sound file and return a reference to it
// -----------------------------------------------------------------------------------------

long platformLoadSoundFile (string theFilename)
{
#ifdef QuickTimeInstalled
	short			myRefNum;
	OSStatus		err = noErr;
	Movie			theMovie;
	FSSpec			theSoundFile;

#ifdef __APPLE__
	CFURLRef		musicURL;
	FSRef			musicfsRef;
#endif


	// get the full path to the sound file
	string fileLocation;
	fileLocation = pathToResourceDirectory() + theFilename;


#ifdef __APPLE__
	// create a URL to the file from the C string
	musicURL = CFURLCreateWithBytes (kCFAllocatorDefault, (UInt8 *)fileLocation.c_str(), fileLocation.length(), kCFStringEncodingASCII, NULL);
	if (musicURL == NULL)
	{
		cout << "error getting URL for sound file " << fileLocation << " (sound file may not exist)";
		return 0L;
	}

	// get a FSRef from the URL
	if (!CFURLGetFSRef(musicURL, &musicfsRef))
	{
		cout << "error getting FSRef for sound file " << fileLocation << " (sound file may not exist)";
		CFRelease(musicURL);
		return 0L;
	}
	
	// get the FSSpec from the FSRef
	if (FSGetCatalogInfo (&musicfsRef, kFSCatInfoNone, NULL, NULL, &theSoundFile, NULL))
	{
		cout << "error getting FSSpec for sound file " << fileLocation << " (sound file may not exist)";
		CFRelease(musicURL);
		return 0L;
	}
		
	// release the URL (not needed any more)
	CFRelease(musicURL);
#endif

#ifdef _WIN32
	// get an FSSpec from the pathname
	if (NativePathNameToFSSpec ((char *)fileLocation.c_str(), &theSoundFile, 0))
	{
		cout << "error getting FSSpec for sound file " << fileLocation << " (sound file may not exist)" << endl;
		return 0L;
	}
#endif
	


	// tell QuickTime to open the sound file
	err = OpenMovieFile(&theSoundFile, &myRefNum, fsRdPerm);
	if (err)
	{
		cout << "OpenMovieFile returned " << (long)err << endl;
		return 0L;
	}
	else
	{
		// tell QuickTime to imagine that this is a movie
		NewMovieFromFile(&theMovie, myRefNum, NULL, (StringPtr)NULL, newMovieActive, NULL);
		if (myRefNum != 0)
			CloseMovieFile(myRefNum);
	}
	
	
	// return the reference to the sound
	return (long)theMovie;

#endif // QuickTimeInstalled
}





// -----------------------------------------------------------------------------------------
// platformUnloadSound()
// -----------------------------------------------------------------------------------------
// stops playing the sound and unloads it
// -----------------------------------------------------------------------------------------

void platformUnloadSound (long theSound)
{
#ifdef QuickTimeInstalled
	DisposeMovie((Movie)theSound);
#endif // QuickTimeInstalled
}




// -----------------------------------------------------------------------------------------
// platformStartSound()
// -----------------------------------------------------------------------------------------
// starts playing a sound
// -----------------------------------------------------------------------------------------

void platformStartSound (long theSound)
{
#ifdef QuickTimeInstalled
	// start giving processing time to the sound player
	StartMovie((Movie)theSound);
#endif // QuickTimeInstalled
}





// -----------------------------------------------------------------------------------------
// platformStopSound()
// -----------------------------------------------------------------------------------------

void platformStopSound (long theSound)
{
#ifdef QuickTimeInstalled
	StopMovie((Movie)theSound);
#endif // QuickTimeInstalled
}




// -----------------------------------------------------------------------------------------
// platformSoundLevel()
// -----------------------------------------------------------------------------------------
// the first parameter is the sound reference number
// the second parameter is a floating point value between 0.0 and 1.0.  1.0 represents the
// maximum sound level
// -----------------------------------------------------------------------------------------

void platformSoundLevel (long theSound, float theLevel)
{
#ifdef QuickTimeInstalled
	if (theLevel > 1.0) theLevel = 1.0;
	else if (theLevel < 0.0) theLevel = 0.0;
	
	int theVolume = int(floor(theLevel * 127.0));
	
	SetMovieVolume((Movie)theSound, theVolume);
#endif // QuickTimeInstalled
}




// -----------------------------------------------------------------------------------------
// platformSetSoundPlayPoint()
// -----------------------------------------------------------------------------------------
// the first parameter is the sound reference number
// the second parameter is a floating point value that repesents the time in seconds that
// the playpoint should be moved to
// -----------------------------------------------------------------------------------------

void platformSetSoundPlayPoint (long theSound, float theTime)
{
#ifdef QuickTimeInstalled
	TimeScale scale = GetMovieTimeScale((Movie)theSound);
	SetMovieTimeValue((Movie)theSound, (long)(scale * theTime));
#endif // QuickTimeInstalled
}




// -----------------------------------------------------------------------------------------
// platformProcessSound()
// -----------------------------------------------------------------------------------------
// called periodically when the animation is running to give time to sound processing
// -----------------------------------------------------------------------------------------

void platformProcessSound()
{
#ifdef QuickTimeInstalled
	MoviesTask(NULL, 0);
#endif // QuickTimeInstalled
}




// -----------------------------------------------------------------------------------------
// platformLoadTextureFile()
// -----------------------------------------------------------------------------------------
// loads a (disk-based) image into a GWorld created without padding (via 
// QTNewGWorldFromPtr (...)) that can be used for packed pixel texturing in OpenGL...
// -----------------------------------------------------------------------------------------

GLuint platformLoadTextureFile(string theFilename, bool generateMipmaps, int *width, int *height)
{
#ifdef QuickTimeInstalled
	GLuint					textureName;				// the "name" by which OpenGL knows the texture
	unsigned char 			*pImageBuffer;				// the buffer that contains the image data 
	GWorldPtr				pGWorld;					// a gworld to load the image into
	
	int						imageDepth;
	
	FSSpec 					fsspecImage;				// FSSpec of the image to load
#ifdef __APPLE__	
	CFURLRef				imageURL;
	FSRef					imagefsRef;
#endif
	
	OSStatus				err = noErr;				// err return value
	long					rowStride;					// length, in bytes, of a pixel row in the image
	GraphicsImportComponent giComp;						// component for importing image
	MatrixRecord			matrix;
	Rect 					rectImage;					// rectangle of source image
	PixMapHandle			hPixMap;					// handle to image pix map
	ImageDescriptionHandle	hImageDesc;					// handle to image description used to get image depth

	long					imageWidth;
	long					imageHeight;
	float					imageAspect;
	long					textureWidth;
	long					textureHeight;



	// get the full path to the texture file
	string fileLocation;
	fileLocation = pathToResourceDirectory() + theFilename;



#ifdef __APPLE__	
	// create a URL to the file from the C string
	imageURL = CFURLCreateWithBytes (kCFAllocatorDefault, (UInt8 *)fileLocation.c_str(), fileLocation.length(), kCFStringEncodingASCII, NULL);
	if (imageURL == NULL)
	{
		cout << "error getting URL for image file (image file may not exist): " << theFilename << endl;
		return 0;
	}


	// get a FSRef from the URL
	if (!CFURLGetFSRef(imageURL, &imagefsRef))
	{
		cout << "error getting FSRef for image file:: " << theFilename << endl;
		CFRelease(imageURL);
		return 0;
	}
	

	// get the FSSpec from the FSRef
	if (FSGetCatalogInfo (&imagefsRef, kFSCatInfoNone, NULL, NULL, &fsspecImage, NULL))
	{
		cout << "error getting FSSpec for image file: " << theFilename << endl;
		CFRelease(imageURL);
		return 0;
	}
	
	
	// release the URL (not needed any more)
	CFRelease(imageURL);
#endif

#ifdef _WIN32
	// get an FSSpec from the pathname
	if (NativePathNameToFSSpec ((char *)fileLocation.c_str(), &fsspecImage, kErrorIfFileNotFound))
	{
		cout << "error getting FSSpec for image file: " << fileLocation << endl;
		return 0;
	}
#endif


	// save the onscreen graphics port
	GDHandle	origDevice;
	CGrafPtr	origPort;
	GetGWorld (&origPort, &origDevice);


	// get an importer for the file
	err = GetGraphicsImporterForFileWithFlags (&fsspecImage, &giComp, kDontUseValidateToFindGraphicsImporter);
	if (err != noErr) return 0;

	
	// get the image bounds
	err = GraphicsImportGetNaturalBounds (giComp, &rectImage); 
	if (err != noErr) return 0;


	// create a handle for the image description and lock it
	hImageDesc = (ImageDescriptionHandle) NewHandle (sizeof (ImageDescriptionHandle)); 
	HLock ((Handle) hImageDesc);


	// retrieve the image description
	err = GraphicsImportGetImageDescription (giComp, &hImageDesc); 
	if (err != noErr) return 0;
	
	
	// find width and height
	imageWidth = (int) (rectImage.right - rectImage.left);
	imageHeight = (int) (rectImage.bottom - rectImage.top);

	
	// now calculate the aspect ratio (width/height), used to restore image correctly
	imageAspect = ((float) imageWidth) / ((float) imageHeight);

	
	// get the image's pixel depth
	imageDepth = (**hImageDesc).depth;

	
	// find nearest acceptable texture size (width and height) for the image
	textureWidth = GetTextureDimFromImageDim (imageWidth);
	textureHeight = GetTextureDimFromImageDim (imageHeight);
	
	
	// pass the optimised (and scaled) size back out to the caller
	*width = textureWidth;
	*height = textureHeight;


	// set texture rectangle for creation of GWorld
#ifdef __APPLE__
	SetRect (&rectImage, 0, 0, (int) textureWidth, (int) textureHeight);
#endif
#ifdef _WIN32
	MacSetRect (&rectImage, 0, 0, (int) textureWidth, (int) textureHeight);
#endif

	// set stride in bytes width of image * 4 bytes per pixel
	rowStride = textureWidth * 4;
	
	// build new buffer exact size of image (stride * height)
	pImageBuffer = (unsigned char *) NewPtr (rowStride * textureHeight); 


	// check we got the buffer we wanted
	if (pImageBuffer == NULL)
	{
		// failed - release the component and return an error
		CloseComponent(giComp);
		return 0;
	}

	
	// create a new gworld using our unpadded buffer, setting the pixel type correctly for the expected image depth
#ifdef __BIG_ENDIAN__
	QTNewGWorldFromPtr (&(pGWorld), k32ARGBPixelFormat, &rectImage, NULL, NULL, 0, pImageBuffer, rowStride);
#else
	QTNewGWorldFromPtr (&(pGWorld), k32RGBAPixelFormat, &rectImage, NULL, NULL, 0, pImageBuffer, rowStride);
#endif
	
	
	// could we allocate the GWorld?
	if (pGWorld == NULL)
	{
		// failed - release the buffer, component and return an error
		DisposePtr ((Ptr) pImageBuffer);
		CloseComponent(giComp);
		return 0;
	}


	// build a transformation matrix to scale the image to the texture size
	// this also flips the image horizontally, so that texture coordinates map correctly
	float	horizontalScale = (float) textureWidth / (float) imageWidth;
	float	verticalScale = (float) textureHeight / (float) imageHeight;

	SetIdentityMatrix (&matrix);
	ScaleMatrix (&matrix, X2Fix(horizontalScale), X2Fix(-verticalScale), 0, 0);
	TranslateMatrix (&matrix, 0, X2Fix((float)textureHeight));


	// set the matrix as the importer matrix
	err = GraphicsImportSetMatrix(giComp, &matrix); 

	// set the destination of the importer component
	if (err == noErr) err = GraphicsImportSetGWorld (giComp, pGWorld, NULL);
	
	// ensure lossless decompression (if the CODEC supports this)
	if (err == noErr) err = GraphicsImportSetQuality(giComp, codecLosslessQuality); 

	if (err != noErr)
	{
		// failed - release the GWorld, buffer, component and return an error
		DisposeGWorld (pGWorld);
		DisposePtr ((Ptr) pImageBuffer);
		CloseComponent(giComp);
		return 0;
	}

	// get the address of the GWorld's pixmap
	hPixMap = GetGWorldPixMap (pGWorld);
	
	// if everything looks good draw the image to the locked pixmap
	if ((hPixMap) && (LockPixels (hPixMap)))
		GraphicsImportDraw (giComp);
	else
	{
		// the pixmap doesn't exist, or we couldn't lock it
		// release the GWorld, buffer, component and return an error
		DisposeGWorld (pGWorld);
		DisposePtr ((Ptr) pImageBuffer);
		CloseComponent(giComp);
		return 0;
	}


	// for images without an alpha channel, initialise the alpha bytes since QuickTime won't
	if (imageDepth < 32)
	{
#ifdef __BIG_ENDIAN__
		for( unsigned char *p = pImageBuffer; p < pImageBuffer + (rowStride * textureHeight); p+=4)
			*p = 0xFF;
#else
		for( unsigned char *p = pImageBuffer+3; p < pImageBuffer + (rowStride * textureHeight) +3; p+=4)
			*p = 0xFF;
#endif
	}

	// unlock the pixmap
	UnlockPixels (hPixMap);
	
	
	// dump the component
	CloseComponent(giComp);

	
	// set image width in groups (pixels), accounts for border this ensures proper image alignment row to row
	glPixelStorei (GL_UNPACK_ROW_LENGTH, textureWidth);


	// generate a "name" for the texture
	glGenTextures (1, &textureName);
	
	
	// create the texture in OpenGL
	glBindTexture(GL_TEXTURE_2D, textureName);



	// tell OpenGL about the texture and have GLU build mipmaps
#ifdef __BIG_ENDIAN__
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pImageBuffer);
#else
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageBuffer);
#endif
	if (generateMipmaps)
#ifdef __BIG_ENDIAN__
	if (gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, textureWidth, textureHeight, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pImageBuffer) != 0)
#else
	if (gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, pImageBuffer) != 0)
#endif
	
	
	// good time to check for errors?
	openGLCheckError();
	

	// finished with the GWorld but not the image buffer (it's used by OpenGL as the "live" image buffer)
	DisposeGWorld (pGWorld);

	// restore the current graphics port we saved earlier
	SetGWorld(origPort, origDevice);


	// all done - return the texture name
	return textureName; 
#endif // QuickTimeInstalled
}


