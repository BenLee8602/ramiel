#ifndef DRAW_H_
#define DRAW_H_


#include "render.h"


void drawRect(unsigned int color, int x1, int y1, int len, int ht);

void drawHzLine(int x1, int x2, int y, unsigned int color);
void drawVertLine(int y1, int y2, int x);
void drawLine(int x1, int y1, int x2, int y2);

void drawVector(Vec3f vec);

void drawTriangleOutline(struct Tri2D* tri);
void drawTriangle(struct Tri2D* tri, unsigned int color);


#endif
