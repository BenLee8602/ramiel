#ifndef CLIP_H_
#define CLIP_H_


#include "render.h"

void connectLines(std::vector<Line2D>& edges);
void clipTriangle(std::vector<Tri2D>& clippedTris, Tri2D triInput);
void clipZaxis(std::vector<Tri3D>& clippedTris, Tri3D triInput);


#endif
