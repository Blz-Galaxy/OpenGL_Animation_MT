// =========================================================================================
// KXC354 - Computer Graphics & Animation, 2012
// Assignment 1 & 2
// 3D Engine Code
// =========================================================================================
//
// Author: Tony Gray
//
// main.h

#ifndef _MAIN_H
#define _MAIN_H


void nextFrame(void);
void drawScene(void);
void keyPress(unsigned char, int x, int y);
void specialKeyPress(int key, int x, int y);
void resizeWindow(int width, int height);
void mouseClick(int button, int state, int x, int y);
void mouseMove(int x, int y);
void captureCameraState(void);
void printCameraState(void);
void animateForNextFrame(float time, long frame);


#endif // _MAIN_H

