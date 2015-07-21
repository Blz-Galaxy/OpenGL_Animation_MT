// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//         Stuart Attenborrow (mouse navigation)
//
// main.cpp



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "constants.h"				// system-wide constants
#include "utility.h"				// general utility functions
#include "platform.h"				// platform-specific additions
#include "object3d.h"				// the object class
#include "light.h"					// light objects
#include "scene.h"					// scene creation and rendering
#include "camera.h"
#include "matrix.h"
#include "snd.h"
#include "fog.h"
#include "viewfrustum.h"

#include "main.h"					// function prototypes for local functions



// -----------------------------------------------------------------------------------------
// global variables
// -----------------------------------------------------------------------------------------

ProgramMode		gProgramMode = kpmAtStart;			// initially, we're not animating
long			gRenderedFrameCounter;				// counts how many frames we've actually drawn
long			gAnimatedFrameCounter;				// counts how many frames we've animated
int				gWindowWidth;						// the current window size
int				gWindowHeight;
float			gCurrentFrameTime;					// the time (since the animation started) for the frame being animated
float			gLastFrameRenderTime;				// the time (since the animation started) that the last frame was rendered

unsigned long	gPolygonCount = 0;					// counts how many polygons get displayed each frame

bool			gShowGlobalAxes = false;			// render the global axes
bool			gShowGlobalGrid = false;			// render the global X-Z grid
bool			gShowLocalAxes = false;				// render the local axes
bool			gShowFaceNormals = false;			// render face normals
bool			gWireFrameOnly = false;				// wireframe or solid view
bool			gAlwaysDrawVertices = false;
bool			gFullScreen = false;				// currently in full screen mode
bool			gLimitFrameRate = true;				// default to throttle the frame rate
bool			gTextOn = false;					// whether to display text initially
bool			gShowParentage = false;				// whether to draw lines from chldren to their parents
bool			gShowLights = false;				// whether to draw light source proxies
GLuint			gFontList = 0;						// the OpenGL font for writing on screen

object3d		*gSky = 0;							// the skybox (init to null, so no skybox at first)
fog             *gFog = 0;							// the fog class (init to null, so no fog at first)
viewfrustum		*gViewFrustum = 0;					// the current view frustum (inited in main, only really need one)
GLuint			gNoiseTexture3d;					// for shaders

cameraClass		gCamera;							// the camera

string			testStageName;						// for assignment 1 only - for displaying the name of the current stage when "animating"


// the list of 3D objects is stored in an STL container
vector<object3d*> gShapeVector;
typedef vector<object3d*>::iterator shapeVectorIterator;



// -----------------------------------------------------------------------------------------
// mouse variables
// -----------------------------------------------------------------------------------------

int mouseState;
int mouseX, mouseY;

float viewVectorLength;			// length of vector from camera to target
vectr cameraLocation;			// the camera's location
vectr cameraRotation;			// the camera's rotation based on the target
								// Y rotation: 	0 degress @ (0, +z)
								// 				90 degrees @ (+x, 0) 
								// 				180 degrees @ (0, -z)
								//				270 degress @ (-x, 0)
vectr targetLocation;			// The camera targets location
vectr cameraViewVector;			// A vector representing the direction the cam is pointing
vectr perpY, perpX;				// Perpendicular vectors to camOffset
vectr newpos;					// Temporary vector when calculating camera rotation
matrix rotationMatrix;
const int X = 0, Y = 1, Z = 2;	// used when accessing array in vec objects




// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	// initialise any platform-specific features
	platformInitialise();
	
	// set the initial window size
	gWindowWidth = kWindowWidth;
	gWindowHeight = kWindowHeight;
	
	// initialise GLUT
	glutInit(&argc, argv);

	
	glutInitWindowSize(gWindowWidth, gWindowHeight);
	glutInitWindowPosition(100, 100);
	
	glutInitDisplayString("double rgb depth=32");
	glutCreateWindow("Assignment 2");				// comment out for initial full-screen
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
		fatal ("Unable to initialise glew");

	
	// register callbacks for GLUT
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyPress);
	glutSpecialFunc(specialKeyPress);
	glutIdleFunc(NULL);
	glutReshapeFunc(resizeWindow);
	
	// register mouse callbacks for GLUT
	mouseState = -1;
	mouseX = mouseY = 0;
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	
	// uncomment these lines to find out more about your OpenGL version
	cout << "OpenGL vendor:    " << (char*) glGetString(GL_VENDOR) << endl;
	cout << "OpenGL renderer:  " << (char*) glGetString(GL_RENDERER) << endl;
	cout << "OpenGL version:   " << (char*) glGetString(GL_VERSION) << endl;


	// construct and initialise the scene
	gViewFrustum = new viewfrustum();
	constructScene();
	gRenderedFrameCounter = 0;
	gAnimatedFrameCounter = 0;
	resetAnimationTimer();
	
#ifdef MOVIE_SHOW
	keyPress('f', 0, 0);
	keyPress('a', 0, 0);
#endif

	// let GLUT manage things from here
	glutMainLoop();
	return 0;
}




// -----------------------------------------------------------------------------------------
// nextFrame
// -----------------------------------------------------------------------------------------
// called by GLUT when it has nothing else to do (ie., this is the idle function)
// -----------------------------------------------------------------------------------------

void nextFrame(void)
{
    static float musicTaskTime = -1.0;
	float timeSinceAnimationStarted = getTimeSinceAnimationStarted();
	
	if (gProgramMode == kpmAnimating)
	{
		// check if it's time to animate a frame
		// get the actual time, and change it to a value that represents the time at which the next frame should be ANIMATED
		float f = (floor (timeSinceAnimationStarted / kTimeBetweenAnimates)) * kTimeBetweenAnimates;
		if (gCurrentFrameTime < f)
			while (gCurrentFrameTime < f)
			{
				// call animateScene enough times to catch up
				animateForNextFrame(gCurrentFrameTime, gAnimatedFrameCounter);
				gAnimatedFrameCounter++;
				gCurrentFrameTime += kTimeBetweenAnimates;
			}
		else
		{
			// check if it's time to render a frame (note that we never animate and render in the same call)
			// get the actual time, and change it to a value that represents the time at which the next frame should be RENDERED
			// note that we always render if rate limiting is turned off
			f = (floor (timeSinceAnimationStarted / kTimeBetweenRedraws)) * kTimeBetweenRedraws;
			if (!gLimitFrameRate || (gLastFrameRenderTime < f))
			{
				// call drawScene once only
				gLastFrameRenderTime = f;
				drawScene();
				gRenderedFrameCounter++;
			}
#ifdef __APPLE__
			else													// on MacOS X, give time back to other apps
				usleep(trunc(kTimeBetweenRedraws * 250000));		// sleep for quarter of the desired frame time
#endif
		}
		
		// ensure that sound gets a chance to be processed 5(ish) times per second
		if (gCurrentFrameTime >= musicTaskTime + 0.20)
		{
			musicTaskTime = gCurrentFrameTime;
			platformProcessSound();
		}
	}
	else if (gProgramMode == kpmFinished)
	{
#ifdef MOVIE_SHOW
		platformWindup();						// platform-specific close down
		exit (0);
#else
		glutIdleFunc(NULL);						// stop calling idle when the animation is finished
		pauseAnimationTimer();					// stop the timer (so it doesn't update if the user resizes the window)
		snd::stopAllSounds();					// and stop playing any audio
		glutSetCursor(GLUT_CURSOR_INHERIT);		// restore the cursor
#endif
	}
}




// -----------------------------------------------------------------------------------------
// drawScene
// -----------------------------------------------------------------------------------------
// this function
// - clears the OpenGL buffer
// - sets up openGL state for drawing (lighting, projection mode, etc)
// - draws the skybox (if one is installed)
// - depth-sorts all objects (but only at the object level, not the face level)
// - draws the global grid and axes (if active)
// - enables fog (if active)
// - ask each object in the object vector to draw itself in opaque mode
// - turns on blending
// - ask each object in the object vector to draw itself in transparent mode
// - displays the statistics if necessary
//
// it is called by NextFrame (the GLUT idle function)
// -----------------------------------------------------------------------------------------

void drawScene(void)
{

	// zero the global polygon counter
	gPolygonCount = 0;
	
	
	// clear the colour buffer and the Z-buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	// select between wireframe and filled polygons
	if (gWireFrameOnly)
		glPolygonMode(GL_FRONT, GL_LINE);
	else
		glPolygonMode(GL_FRONT, GL_FILL);
	
	
	// draw the skybox if one is has been loaded (before any other drawing)
	if (gSky) gSky->privateDraw(kOpaqueMode);


	// set up for perspective projection
	glMatrixMode(GL_PROJECTION);
	gViewFrustum->tellGL();
	glMatrixMode(GL_MODELVIEW);


	// calculate the distance from the camera to each object on the shapeVector, then distance-sort
	// so that the furthest objects are at the start of the list
	shapeVectorIterator svi;
	for (svi = gShapeVector.begin(); svi != gShapeVector.end(); svi++)
		(*svi)->calculateDistanceToCamera();
	sort(gShapeVector.begin(), gShapeVector.end(), objectToCameraDistanceComparison);



	// draw the global grid and axes if necessary
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	gCamera.tellGL();
	glLineWidth(2);
	
	if (gShowGlobalAxes)
	{
		glBegin(GL_LINES);
		glColor4f(1.0, 1.0, 0, 1.0);
		glVertex3f(-200.0, 0, 0); glVertex3f(200, 0, 0);
		glVertex3f(0.0, -200, 0); glVertex3f(0, 200, 0);
		glVertex3f(0.0, 0, -200); glVertex3f(0, 0, 200);
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnd();
	}
	
	if (gShowGlobalGrid)
	{
		glBegin(GL_LINES);
		glColor4f(0.5, 0.5, 0.5, 1.0);
		for (float x=-200.0; x<=200.0; x+=10.0)
		{
			glVertex3f(x, 0.0, -200.0);
			glVertex3f(x, 0.0, +200.0);
		}
		for (float z=-200.0; z<=200.0; z+=10.0)
		{
			glVertex3f(-200.0, 0.0, z);
			glVertex3f(+200.0, 0.0, z);
		}
		glColor4f(1.0, 1.0, 1.0, 1.0);
		glEnd();
	}
	glLineWidth(1);
	glEnable(GL_LIGHTING);
	

	// enable fog if required
	if (gFog)
	{
		glEnable(GL_FOG);
		gFog->tellGL();
	}

	// setup up for normal drawing
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	
	// send the light settings to the pipeline
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	light::tellAllGL();


	// enable "on/off" transparency ("cookie cutter alpha")
	glAlphaFunc(GL_GREATER, 0.99f);
	glEnable(GL_ALPHA_TEST);


	// tell all objects to draw themselves, in opaque mode
	for (svi = gShapeVector.begin(); svi != gShapeVector.end(); svi++)
		(*svi)->draw(kOpaqueMode);


	// draw light proxies if required
	if (gShowLights)
	{
		glDisable(GL_LIGHTING);
		light::drawAll();
		glEnable(GL_LIGHTING);
	}

	// lock the depth buffer (because transparent objects in front of other
	// objects shouldn't bring the depth values forward)
	glDepthMask(GL_FALSE);
	

	// turn off "on/off" transparency, and enable blending for the transparent pass
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	// tell all objects to draw themselves, in transparent mode
	// (remember, these are depth sorted, so should draw correctly)
	for (svi = gShapeVector.begin(); svi != gShapeVector.end(); svi++)
		(*svi)->draw(kTransparentMode);


	// now we've drawn the transparent objects, unlock the depth buffer again
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);


	// turn off fog if it was turned on
	if (gFog)
		glDisable(GL_FOG);
	

	// visualise parentage
	if (gShowParentage)
	{
		glLoadIdentity();
		gCamera.tellGL();
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		for (svi = gShapeVector.begin(); svi != gShapeVector.end(); svi++)
			(*svi)->drawParentLink();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	
	if (gTextOn)
	{
		// draw stats
		stringstream tempString;
		tempString.precision(2);
		tempString	<< "Frame: " << gRenderedFrameCounter << ", Polygons: " << gPolygonCount 
					<< ", Time: " << fixed << gCurrentFrameTime;
		tempString.precision(1);
		tempString	<< fixed << ", Frame rate: " << calculateFrameRate();
		drawText (tempString.str(), 10, 20);
	}


	// show stage (assignment 1 test function)
	drawText (testStageName, 10, 5);
	
    
	glutSwapBuffers();

}




// -----------------------------------------------------------------------------------------
// keyPress
// -----------------------------------------------------------------------------------------
// This function is called by GLUT when the user presses a key on the keyboard.  The 
// values 'x' and 'y' contain the mouse location (from the top left of the window) at the
// time of the keypress
// -----------------------------------------------------------------------------------------

void keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
		// ---------------------------------------------------------------------------------
		// keypress 'a' - animate
		// ---------------------------------------------------------------------------------
		case 'a':
			gProgramMode = kpmPaused;			// in case we were already animating, pretend to stop
			snd::preloadAllSounds();			// try to get all sounds into memory before starting
			gRenderedFrameCounter = 0;			// reset the frame counters
			gAnimatedFrameCounter = 0;
			gCurrentFrameTime = 0;				// reset the frame times
			gLastFrameRenderTime = -1;
			resetAnimationTimer();				// restart the timer
			gProgramMode = kpmAnimating;		// now in animation mode
			glutIdleFunc(nextFrame);			// this forces GLUT to ask us to redraw when there's nothing better to do
			glutSetCursor(GLUT_CURSOR_NONE);	// hide the cursor
			

			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'f' - fullscreen/window toggle
		// ---------------------------------------------------------------------------------
		case 'f':
			if (gFullScreen)
			{
				glutReshapeWindow(kWindowWidth, kWindowHeight);
				glutPositionWindow(100,100);
			}
			else
				glutFullScreen();
			gFullScreen = !gFullScreen;
			break;
			
			
		// ---------------------------------------------------------------------------------
		// keypress 'F' - "FAST"- unlimit/limit frame rate
		// note that we don't limit the frame rate on Windows, as it doesn't have 
		// accurate sleep timers
		// ---------------------------------------------------------------------------------
		case 'F':
#ifdef __APPLE__
			gLimitFrameRate = !gLimitFrameRate;
#endif
			break;
			
			
		// ---------------------------------------------------------------------------------
		// keypress 'p' - pause
		// ---------------------------------------------------------------------------------
		case 'p':
			if (gProgramMode == kpmAnimating)
			{
				glutSetCursor(GLUT_CURSOR_INHERIT);
				gProgramMode = kpmPaused;
				pauseAnimationTimer();
				snd::pauseAllSounds();
				glutIdleFunc(NULL);
			}
			else if (gProgramMode == kpmPaused)
			{
				glutSetCursor(GLUT_CURSOR_NONE);
				gProgramMode = kpmAnimating;
				unpauseAnimationTimer();
				snd::unpauseAllSounds();
				glutIdleFunc(nextFrame);
			}
			break;
			
			
		// ---------------------------------------------------------------------------------
		// keypress 'P' - show parentage toggle
		// ---------------------------------------------------------------------------------
		case 'P':
			gShowParentage = !gShowParentage;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'l' - light proxy toggle
		// ---------------------------------------------------------------------------------
		case 'l':
			gShowLights = !gShowLights;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 't' - text display toggle
		// ---------------------------------------------------------------------------------
		case 't':
			gTextOn = !gTextOn;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'v' - draw vertices toggle
		// ---------------------------------------------------------------------------------
		case 'v':
			gAlwaysDrawVertices = !gAlwaysDrawVertices;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
		
		
		// ---------------------------------------------------------------------------------
		// keypress 'w' - wireframe toggle
		// ---------------------------------------------------------------------------------
		case 'w':
			gWireFrameOnly = !gWireFrameOnly;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
		
		
		// ---------------------------------------------------------------------------------
		// keypress 'c' - print out camera information
		// ---------------------------------------------------------------------------------
		case 'c':
			printCameraState();
			break;		
		
	
		// ---------------------------------------------------------------------------------
		// keypress 'i/I' - move camera in 1/0.1 units
		// ---------------------------------------------------------------------------------
		case 'i':
			gCamera.moveBy(0, 0, -1);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
		case 'I':
			gCamera.moveBy(0, 0, -0.1);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'o/O' - move camera out 1/0.1 units
		// ---------------------------------------------------------------------------------
		case 'o':
			gCamera.moveBy(0, 0, 1);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
		case 'O':
			gCamera.moveBy(0, 0, 0.1);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'X/x' - turn on the global/local axis markers
		// ---------------------------------------------------------------------------------
		case 'X':
			gShowGlobalAxes = !gShowGlobalAxes;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
		case 'x':
			gShowLocalAxes = !gShowLocalAxes;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'g' - turn on the XZ plane grid
		// ---------------------------------------------------------------------------------
		case 'g':
			gShowGlobalGrid = !gShowGlobalGrid;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'n' - turn on the display of normals
		// ---------------------------------------------------------------------------------
		case 'n':
			gShowFaceNormals = !gShowFaceNormals;
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// keypress 'q', 'ESC' - quot
		// ---------------------------------------------------------------------------------
		case 27:
		case 'q':
			platformWindup();					// platform-specific close down
			exit (0);
			break;
	}
}




// -----------------------------------------------------------------------------------------
// specialKeyPress
// -----------------------------------------------------------------------------------------
// This function is called by GLUT when the user presses a "special" key on the keyboard,
// like an arrow key (special keys don't trigger the default keypress function).
// NOTE - these functions don't generate the expected results unless the camera is
// in front of the scene - if it is behind the scene looking out, the left/right axes
// will be flipped.
// NOTE - these functions only changes the target point of the camera by 1.0 - they don't
// "rotate" the viewpoint around the camera as you might expect.
// -----------------------------------------------------------------------------------------

void specialKeyPress(int key, int x, int y)
{
	switch (key)
	{
		// ---------------------------------------------------------------------------------
		// left arrow - move left by 1 unit
		// ---------------------------------------------------------------------------------
		case GLUT_KEY_LEFT:
			gCamera.moveBy(-1.0, 0, 0);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	
	
		// ---------------------------------------------------------------------------------
		// right arrow - look right
		// ---------------------------------------------------------------------------------
		case GLUT_KEY_RIGHT:
			gCamera.moveBy(1.0, 0, 0);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
			
			
		// ---------------------------------------------------------------------------------
		// up arrow - look up
		// ---------------------------------------------------------------------------------
		case GLUT_KEY_UP:
			gCamera.moveBy(0, 1.0, 0);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
			
			
		// ---------------------------------------------------------------------------------
		// down arrow - look down
		// ---------------------------------------------------------------------------------
		case GLUT_KEY_DOWN:
			gCamera.moveBy(0, -1.0, 0);
			if (gProgramMode != kpmAnimating) glutPostRedisplay();
			break;
	}
}



// -----------------------------------------------------------------------------------------
// resizeWindow
// -----------------------------------------------------------------------------------------
// this function is called by GLUT when the user resizes the window
// -----------------------------------------------------------------------------------------

void resizeWindow(int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;

	glMatrixMode(GL_PROJECTION);
	gViewFrustum->tellGL();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	
	if (gProgramMode == kpmAnimating)			// if animating, ensure nextframe gets called...
	{
		nextFrame();
//		animateForNextFrame(gCurrentFrameTime, gAnimatedFrameCounter);
//		gAnimatedFrameCounter++;
//		gCurrentFrameTime += kTimeBetweenAnimates;
	}
}



// -----------------------------------------------------------------------------------------
// MouseClick
// -----------------------------------------------------------------------------------------
// this function is called by GLUT when the user has clicks/unclicks the mouse
// all the camera information is captured at this point - ready for using in MouseMove
// (written by Stuart Attenborrow, Aug 2005)
// -----------------------------------------------------------------------------------------

void mouseClick(int button, int state, int x, int y)
{
	switch (state)
	{
		case (GLUT_DOWN):
			
			mouseState = button;
			captureCameraState();
			
			if (button != GLUT_LEFT_BUTTON)
			{
				// Only normalise offsets if we're zooming/panning
				cameraViewVector.reduceToUnit();
				
				// Create the X panning vector
				// Make vector (1,0,0)
				perpX.set(1,0,0);
				// Rotate vector by RY
				rotationMatrix.setRotateY(cameraRotation.data[Y]);
				perpX = rotationMatrix * perpX;
				
				
				// Create the Y panning vector
				// Reverse Y rotation to return points to z,y plane
				rotationMatrix.setRotateY(-cameraRotation.data[Y]);
				// Perform dot product rotation
				perpY = rotationMatrix * cameraViewVector;
				// Now find perpendicular vector
				perpY.set( perpY.data[X], -perpY.data[Z], perpY.data[Y]);
				// Reset rotation and return new vector to original Y rotation
				rotationMatrix.setRotateY(cameraRotation.data[Y]);
				perpY = rotationMatrix * perpY;
			}			
			
			break;
		
		case (GLUT_UP):
			mouseState = -1;
			mouseX = mouseY = 0;
	}
}



// -----------------------------------------------------------------------------------------
// MouseMove
// -----------------------------------------------------------------------------------------
// this function is called by GLUT when the user has clicked and is dragging
// it will determine what camera movement to make based on the mouse button clicked
// (written by Stuart Attenborrow, Aug 2005)
// -----------------------------------------------------------------------------------------

void mouseMove(int x, int y)
{
	// Create relative mouse locations
	float offsetX = x - mouseX;
	float offsetY = y - mouseY;
	
	// if this is our first move, set the first mouse click to the "origin",
	// therefore all other mouse movements can be relative to where the user
	// first clicked the mouse :)
	if (mouseX == 0 && mouseY == 0)
	{
		mouseX = x;
		mouseY = y;
	}
		
	// Left mouse button movement = rotate the camera
	// This is a lot more computationally expensive than panning and zooming
	// since matrix/vector mulitplications are done for every pixel moved
	// with the mouse (whereas pan/zoom are done once initially)
	else if (mouseState == GLUT_LEFT_BUTTON)
	{
		float angle = cameraRotation.data[Y] - offsetX/2;
		
		// rotate around Y first
		rotationMatrix.setRotateY(-offsetX/2);
		newpos = rotationMatrix * cameraViewVector;
		
		// Set bounds on up/down rotation so the user doesn't 'flip' over
		if (cameraRotation.data[X] + offsetY > 80)
			offsetY = 80 - cameraRotation.data[X];
		else if (cameraRotation.data[X] + offsetY < -80)
			offsetY = -80 - cameraRotation.data[X];
		
		// now rotate around X,Z
		rotationMatrix.setRotateY(-angle);
		newpos = rotationMatrix * newpos;
		rotationMatrix.setRotateX(-offsetY);
		newpos = rotationMatrix * newpos;
		rotationMatrix.setRotateY(angle);
		newpos = rotationMatrix * newpos;

		gCamera.setPosition(
			targetLocation.data[X] + newpos.data[X],
			targetLocation.data[Y] + newpos.data[Y],
			targetLocation.data[Z] + newpos.data[Z]);
	}
	
	// Middle mouse button movement = pan
	else if (mouseState == GLUT_MIDDLE_BUTTON)
	{
		// Slow down the panning motion
		// camVec*0.002 has been found to give the best panning speed
		offsetX *= (viewVectorLength * 0.002);
		offsetY *= (viewVectorLength * 0.002);
		
		// Move along plane perpendicular to viewing vector
		newpos.set(
			-perpY.data[X] * offsetY - perpX.data[X] * offsetX,
			-perpY.data[Y] * offsetY - perpX.data[Y] * offsetX,
			-perpY.data[Z] * offsetY - perpX.data[Z] * offsetX);
		
		gCamera.setPosition( 
			cameraLocation.data[X] + newpos.data[X], 
			cameraLocation.data[Y] + newpos.data[Y], 
			cameraLocation.data[Z] + newpos.data[Z]);
		
		gCamera.setTarget( 
			targetLocation.data[X] + newpos.data[X], 
			targetLocation.data[Y] + newpos.data[Y], 
			targetLocation.data[Z] + newpos.data[Z]);
	}
	
	// Right mouse button movement = camera zoom
	else if (mouseState == GLUT_RIGHT_BUTTON)
	{
		// zoom by 0.1 units every pixel
		float step = offsetY * 0.1;
		gCamera.setPosition(
			cameraLocation.data[X] + cameraViewVector.data[X] * step, 
			cameraLocation.data[Y] + cameraViewVector.data[Y] * step, 
			cameraLocation.data[Z] + cameraViewVector.data[Z] * step);
	}
	
	// Redraw the display after moving
	if (gProgramMode != kpmAnimating) glutPostRedisplay();
}




// -----------------------------------------------------------------------------------------
// CaptureCameraState/PrintCameraState
// -----------------------------------------------------------------------------------------
// helper functions for the mouse controls above
// (written by Stuart Attenborrow, Aug 2005)
// -----------------------------------------------------------------------------------------

void captureCameraState()
{
	// Get camera locations
	gCamera.getPosition(cameraLocation);
	gCamera.getTarget(targetLocation);
	
	// Find vector between camera and target
	cameraViewVector.set(
		cameraLocation.data[X] - targetLocation.data[X],
		cameraLocation.data[Y] - targetLocation.data[Y],
		cameraLocation.data[Z] - targetLocation.data[Z]);
	
	// Find vector length using pythag
	viewVectorLength = cameraViewVector.length(); 
	
	// Find rotations of camera in relation to target
	cameraRotation.set(
		atan_d(cameraViewVector.data[Y] / cameraViewVector.data[Z]),
		atan_d(cameraViewVector.data[Z] / cameraViewVector.data[X]),
		atan_d(cameraViewVector.data[Y] / cameraViewVector.data[X]));
	
	// Make Y rotation absolute: 0 -> 360
	cameraRotation.data[Y] = (cameraViewVector.data[X] > 0) ? 
		(90 - cameraRotation.data[Y]) : (270 - cameraRotation.data[Y]);
	
	// Camera rotations X & Z found to be incorrect
	rotationMatrix.setRotateY(-cameraRotation.data[Y]);
	newpos = rotationMatrix * cameraViewVector;
	cameraRotation.set(
		atan_d(newpos.data[Y] / newpos.data[Z]), 
		cameraRotation.data[Y], 
		atan_d(newpos.data[Y] / newpos.data[Z]));
}

void printCameraState()
{
	captureCameraState();
	cout.precision(3);
	cout << fixed;
	cout << "camera position - " << cameraLocation.data[X] << ", " << cameraLocation.data[Y] << ", " << cameraLocation.data[Z] << endl;
	cout << "camera target - " << targetLocation.data[X] << ", " << targetLocation.data[Y] << ", " << targetLocation.data[Z] << endl;
}




// -----------------------------------------------------------------------------------------
// animateForNextFrame
// -----------------------------------------------------------------------------------------
// This function is called to animate objects in the scene
// DO NOT MODIFY THIS FUNCTION - IT WILL BE USED TO ASSIST THE MARKING PROCESS
// -----------------------------------------------------------------------------------------
extern object3d		*test_opencube;
extern object3d		*test_opensphere;
extern object3d		*test_opentorus;
extern object3d		*test_extrusion1;
extern object3d		*test_extrusion2;
extern object3d		*test_sweep1;
extern object3d		*test_sweep2;
extern object3d		*test_composite1;
extern object3d		*test_composite2;

void animateForNextFrame(float time, long frame)
{
	shapeVectorIterator svi;
	
	if (frame == 0)
	{
		// hide all objects at the start and position them at the origin
		for (svi = gShapeVector.begin(); svi != gShapeVector.end(); svi++)
			if (!(*svi)->getParent()) 		
			{
				(*svi)->hide();
				(*svi)->setPosition(0, 0, 0);
			}
	}
	
	// rotate all objects that don't have parents (should be all composite/standlone objects)
	for (svi = gShapeVector.begin(); svi != gShapeVector.end(); svi++)
		if (!(*svi)->getParent()) (*svi)->setRotation('y', time*90);
	
	
	switch (frame)
	{
		case 0:
			if (test_opencube) test_opencube->show();
			testStageName.assign("opencube");
			break;
			
		case 500:
			if (test_opencube) test_opencube->hide();
			if (test_opensphere) test_opensphere->show();
			testStageName.assign("opensphere");
			break;
			
		case 1000:
			if (test_opensphere) test_opensphere->hide();
			if (test_opentorus) test_opentorus->show();
			testStageName.assign("opentorus");
			break;
			
		case 1500:
			if (test_opentorus) test_opentorus->hide();
			if (test_extrusion1) test_extrusion1->show();
			testStageName.assign("extrusion 1");
			break;
			
		case 2000:
			if (test_extrusion1) test_extrusion1->hide();
			if (test_extrusion2) test_extrusion2->show();
			testStageName.assign("extrusion 2");
			break;
			
		case 2500:
			if (test_extrusion2) test_extrusion2->hide();
			if (test_sweep1) test_sweep1->show();
			testStageName.assign("sweep 1");
			break;
			
		case 3000:
			if (test_sweep1) test_sweep1->hide();
			if (test_sweep2) test_sweep2->show();
			testStageName.assign("sweep 2");
			break;
			
		case 3500:
			if (test_sweep2) test_sweep2->hide();
			if (test_composite1) test_composite1->show();
			testStageName.assign("composite 1");
			break;
			
		case 4000:
			if (test_composite1) test_composite1->hide();
			if (test_composite2) test_composite2->show();
			testStageName.assign("composite 2");
			break;
			
		case 4500:
			testStageName.assign("done");
			break;
			
		case 4501:
			gProgramMode = kpmFinished;
	}
	
}






