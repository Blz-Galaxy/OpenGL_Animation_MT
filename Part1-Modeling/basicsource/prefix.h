// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// prefix.h
//
// this includes the prefix headers
// this file is precompiled to provide faster compilations

#include <glew.h>

#ifdef __APPLE__ // Mac-specific includes
	#define QuickTimeInstalled
	#include <CoreServices/CoreServices.h>
	#include <QuickTime/QuickTime.h>
	#include <QuickTime/Movies.h>
	#include <QuickTime/QuickTimeComponents.h>
	#include <QuickTime/QTML.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
	#include <GLUT/glut.h>
	#include <sys/param.h>
#endif


#ifdef _WIN32
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
	#include <time.h>
	// comment out the following lines if you do not have QuickTime and the QuickTime SDK installed on Windows
	// this will disable texture and sound support
	#define QuickTimeInstalled
	#include <QTML.h>
	#include <Movies.h>
	#include <CoreFoundation.h>
	#include <FixMath.h>
#endif


// STL bits and pieces

#ifdef __cplusplus
	#include <string>
	#include <cmath>
	#include <cstdio>
	#include <cstdlib>
	#include <vector>
	#include <sstream>
	#include <fstream>
	#include <iostream>
	#include <list>
	#include <functional>
	#include <algorithm>
	using namespace std;
#endif

