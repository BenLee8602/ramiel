#ifndef UTIL_H_
#define UTIL_H_


#include "vec.h"


// misc
int range(int num, int min, int max);
void swapInt(int* num1, int* num2);
void swapFloat(float* num1, float* num2);
int rgbToDec(int r, int g, int b);
struct Vec2 getScreenCoords(Vec3f* pt3D);


// raster
void sortTriToRaster(struct Tri2D* tri, struct Tri3D* triDepth);


// vector
Vec3f addVec3(Vec3f aVec, Vec3f bVec);
Vec3f subtractVec3(Vec3f aVec, Vec3f bVec);

Vec3f scalarMultVec3(Vec3f in, float mult);
Vec3f scalarDivVec3(Vec3f in, float divisor);

float dotProductVec3(Vec3f aVec, Vec3f bVec);
Vec3f crossProductVec3(Vec3f aVec, Vec3f bVec);

Vec3f normalizeVector(Vec3f in);
float similarityVec3(Vec3f aVec, Vec3f bVec);

Vec3f multiplyMatrixVec3(Matrix3* mat, Vec3f* in);


#endif
