#include "util.h"


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


int rgbToDec(int r, int g, int b) {
	return (r << 16) + (g << 8) + b;
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


struct Vec3f addVec3(struct Vec3f aVec, struct Vec3f bVec) {
	struct Vec3f out = { 0.0f };
	out.x = aVec.x + bVec.x;
	out.y = aVec.y + bVec.y;
	out.z = aVec.z + bVec.z;
	return out;
}

struct Vec3f subtractVec3(struct Vec3f aVec, struct Vec3f bVec) {
	struct Vec3f out = { 0.0f };
	out.x = aVec.x - bVec.x;
	out.y = aVec.y - bVec.y;
	out.z = aVec.z - bVec.z;
	return out;
}


struct Vec3f scalarMultVec3(struct Vec3f in, float mult) {
	struct Vec3f out = { 0.0f };
	out.x = in.x * mult;
	out.y = in.y * mult;
	out.z = in.z * mult;
	return out;
}


struct Vec3f scalarDivVec3(struct Vec3f in, float divisor) {
	struct Vec3f out = { 0.0f };
	out.x = in.x / divisor;
	out.y = in.y / divisor;
	out.z = in.z / divisor;
	return out;
}


float dotProductVec3(struct Vec3f aVec, struct Vec3f bVec) {
	return aVec.x * bVec.x + aVec.y * bVec.y + aVec.z * bVec.z;
}


struct Vec3f crossProductVec3(struct Vec3f aVec, struct Vec3f bVec) {
	struct Vec3f out = { 0.0f };
	out.x = aVec.y * bVec.z - aVec.z * bVec.y;
	out.y = aVec.z * bVec.x - aVec.x * bVec.z;
	out.z = aVec.x * bVec.y - aVec.y * bVec.x;
	return out;
}


struct Vec3f normalizeVector(struct Vec3f in) {
	struct Vec3f out = { 0.0f };
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


struct Vec3f multiplyMatrixVec3(struct Matrix3* mat, struct Vec3f* in) {
	struct Vec3f out = { 0.0f };
	out.x = in->x * mat->num[0][0] + in->y * mat->num[0][1] + in->z * mat->num[0][2];
	out.y = in->x * mat->num[1][0] + in->y * mat->num[1][1] + in->z * mat->num[1][2];
	out.z = in->x * mat->num[2][0] + in->y * mat->num[2][1] + in->z * mat->num[2][2];
	return out;
}


void connectLines(std::vector<Line>& edges) {
	// connect distance between end of line and
	// start of adjacent line by inserting new line
	Vec2 pt1;
	Vec2 pt2;
	for (int a = 0; a < edges.size() - 1; a++) {
		pt1 = edges[a].end;
		pt2 = edges[a + 1].start;
		if (pt1.x != pt2.x || pt1.y != pt2.y) {
			edges.insert(edges.begin() + a + 1, Line(pt1, pt2));
		}
	}
	// connect final line in list to first
	pt1 = edges[edges.size() - 1].end;
	pt2 = edges[0].start;
	if (pt1.x != pt2.x || pt1.y != pt2.y) {
		edges.push_back(Line(pt1, pt2));
	}
}
