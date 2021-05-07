#ifndef UTIL_H_
#define UTIL_H_


#include "render.h"


int range(int num, int min, int max) {
	if (num < min) return min;
	if (num > max) return max;
	return num;
}


void swapInt(int* num1, int* num2) {
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}


void sortThreeIntsAscending(int* num1, int* num2, int* num3) {
	if (*num1 > *num2) swapInt(num1, num2);
	if (*num1 > *num3) swapInt(num1, num3);
	if (*num2 > *num3) swapInt(num2, num3);
}


void sortTriToRaster(struct Tri2D* tri) {
	// sort points by y ascending
	if (tri->pts[0].y > tri->pts[1].y) {
		swapInt(&tri->pts[0].y, &tri->pts[1].y);
		swapInt(&tri->pts[0].x, &tri->pts[1].x);
	}
	if (tri->pts[0].y > tri->pts[2].y) {
		swapInt(&tri->pts[0].y, &tri->pts[2].y);
		swapInt(&tri->pts[0].x, &tri->pts[2].x);
	}
	if (tri->pts[1].y > tri->pts[2].y) {
		swapInt(&tri->pts[1].y, &tri->pts[2].y);
		swapInt(&tri->pts[1].x, &tri->pts[2].x);
	}
}


struct Vec3f vectorMatrixMult3(struct Matrix3* mat, struct Vec3f* in) {
	struct Vec3f out = { 0.0f };
	out.x = in->x * mat->num[0][0] + in->y * mat->num[0][1] + in->z * mat->num[0][2];
	out.y = in->x * mat->num[1][0] + in->y * mat->num[1][1] + in->z * mat->num[1][2];
	out.z = in->x * mat->num[2][0] + in->y * mat->num[2][1] + in->z * mat->num[2][2];
	return out;
}


#endif
