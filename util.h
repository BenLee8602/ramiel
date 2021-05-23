#ifndef UTIL_H_
#define UTIL_H_


#include "render.h"


// misc
int range(int num, int min, int max);
void swapInt(int* num1, int* num2);
int rgbToDec(int r, int g, int b);


// raster
void sortTriToRaster(struct Tri2D* tri);


// vector
struct Vec3f addVec3(struct Vec3f aVec, struct Vec3f bVec);
struct Vec3f subtractVec3(struct Vec3f aVec, struct Vec3f bVec);

struct Vec3f scalarMultVec3(struct Vec3f in, float mult);
struct Vec3f scalarDivVec3(struct Vec3f in, float divisor);

float dotProductVec3(struct Vec3f aVec, struct Vec3f bVec);
struct Vec3f crossProductVec3(struct Vec3f aVec, struct Vec3f bVec);

struct Vec3f normalizeVector(struct Vec3f in);
float similarityVec3(Vec3f aVec, Vec3f bVec);

struct Vec3f multiplyMatrixVec3(struct Matrix3* mat, struct Vec3f* in);


// clipping
void connectLines(std::vector<Line>& edges);


#endif
