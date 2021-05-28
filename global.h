#ifndef GLOBAL_H_
#define GLOBAL_H_


#include "camera.h"

#define MAX_RENDER_DISTANCE 1000

extern void* memory;
extern void* zBuffer;

extern int winSizeX;
extern int winSizeY;

extern float winMidX;
extern float winMidY;

extern float dTime;

extern class Camera cam;


#endif
