#include "global.h"
#include "util.h"
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


void swapFloat(float* num1, float* num2) {
	float temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}


int rgbToDec(int r, int g, int b) {
	return (r << 16) + (g << 8) + b;
}


Vec2 getScreenCoords(Vec3f* pt3D) {
	Vec3f in = { 0.0f };
	Vec3f out = { 0.0f };

	in.x = pt3D->x;
	in.y = pt3D->y;
	in.z = pt3D->z;

	// adjust for camera position
	out.x = in.x - cam.pos.x;
	out.y = in.y - cam.pos.y;
	out.z = in.z - cam.pos.z;
	in = out;

	// adjust for camera rotation
	out.x = in.x * cam.cosz + in.y * -cam.sinz;
	out.y = in.x * cam.sinz + in.y * cam.cosz;
	in = out;

	out.x = in.x * cam.cosy + in.z * cam.siny;
	out.z = in.x * -cam.siny + in.z * cam.cosy;
	in = out;

	out.y = in.y * cam.cosx + in.z * -cam.sinx;
	out.z = in.y * cam.sinx + in.z * cam.cosx;

	// project to screen
	Vec2 screenPt = { 0 };
	if (out.z != 0) {
		screenPt.x = (int)(out.x * winSizeX / out.z + winMidX);
		screenPt.y = (int)(out.y * winSizeX / out.z + winMidY);
	}

	return screenPt;
}


void sortTriToRaster(Tri2D* tri, Tri3D* triDepth) {
	// sort points by y ascending
	if (tri->pts[0].y > tri->pts[1].y) {
		swapInt(&tri->pts[0].x, &tri->pts[1].x);
		swapInt(&tri->pts[0].y, &tri->pts[1].y);
		
		swapFloat(&triDepth->pts[0].x, &triDepth->pts[1].x);
		swapFloat(&triDepth->pts[0].y, &triDepth->pts[1].y);
		swapFloat(&triDepth->pts[0].z, &triDepth->pts[1].z);
	}

	if (tri->pts[0].y > tri->pts[2].y) {
		swapInt(&tri->pts[0].x, &tri->pts[2].x);
		swapInt(&tri->pts[0].y, &tri->pts[2].y);

		swapFloat(&triDepth->pts[0].x, &triDepth->pts[2].x);
		swapFloat(&triDepth->pts[0].y, &triDepth->pts[2].y);
		swapFloat(&triDepth->pts[0].z, &triDepth->pts[2].z);
	}

	if (tri->pts[1].y > tri->pts[2].y) {
		swapInt(&tri->pts[1].x, &tri->pts[2].x);
		swapInt(&tri->pts[1].y, &tri->pts[2].y);

		swapFloat(&triDepth->pts[1].x, &triDepth->pts[2].x);
		swapFloat(&triDepth->pts[1].y, &triDepth->pts[2].y);
		swapFloat(&triDepth->pts[1].z, &triDepth->pts[2].z);
	}
}


Vec3f addVec3(Vec3f aVec, Vec3f bVec) {
	Vec3f out = { 0.0f };
	out.x = aVec.x + bVec.x;
	out.y = aVec.y + bVec.y;
	out.z = aVec.z + bVec.z;
	return out;
}

Vec3f subtractVec3(Vec3f aVec, Vec3f bVec) {
	Vec3f out = { 0.0f };
	out.x = aVec.x - bVec.x;
	out.y = aVec.y - bVec.y;
	out.z = aVec.z - bVec.z;
	return out;
}


Vec3f scalarMultVec3(Vec3f in, float mult) {
	Vec3f out = { 0.0f };
	out.x = in.x * mult;
	out.y = in.y * mult;
	out.z = in.z * mult;
	return out;
}


Vec3f scalarDivVec3(Vec3f in, float divisor) {
	Vec3f out = { 0.0f };
	out.x = in.x / divisor;
	out.y = in.y / divisor;
	out.z = in.z / divisor;
	return out;
}


float dotProductVec3(Vec3f aVec, Vec3f bVec) {
	return aVec.x * bVec.x + aVec.y * bVec.y + aVec.z * bVec.z;
}


Vec3f crossProductVec3(Vec3f aVec, Vec3f bVec) {
	Vec3f out = { 0.0f };
	out.x = aVec.y * bVec.z - aVec.z * bVec.y;
	out.y = aVec.z * bVec.x - aVec.x * bVec.z;
	out.z = aVec.x * bVec.y - aVec.y * bVec.x;
	return out;
}


Vec3f normalizeVector(Vec3f in) {
	Vec3f out = { 0.0f };
	float mag = 0.0f;
	mag = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
	out = scalarDivVec3(in, mag);
	return out;
}


float similarityVec3(Vec3f aVec, Vec3f bVec) {
	// return similarity of vector direction
	// 0.0f = opposite dir
	// 0.5f = perpindicular
	// 1.0f = equal dir
	float similarity = 0.0f;
	aVec = normalizeVector(aVec);
	bVec = normalizeVector(bVec);
	similarity = dotProductVec3(aVec, bVec);
	similarity = (similarity + 1.0f) / 2.0f;
	return similarity;
}


Vec3f multiplyMatrixVec3(Matrix3* mat, Vec3f* in) {
	Vec3f out = { 0.0f };
	out.x = in->x * mat->num[0][0] + in->y * mat->num[0][1] + in->z * mat->num[0][2];
	out.y = in->x * mat->num[1][0] + in->y * mat->num[1][1] + in->z * mat->num[1][2];
	out.z = in->x * mat->num[2][0] + in->y * mat->num[2][1] + in->z * mat->num[2][2];
	return out;
}
