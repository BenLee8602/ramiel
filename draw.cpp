#include <windows.h>
#include <algorithm>
#include "draw.h"
#include "global.h"
#include "util.h"









#include <stdio.h>


void drawRect(unsigned int color, int x1, int y1, int len, int ht) {
	unsigned int* pixel = (unsigned int*)memory;
	pixel += x1 + y1 * winSizeX;
	for (int y = 0; y < ht; y++) {
  		for (int x = 0; x < len; x++) {
			*pixel++ = color;
		}
		pixel += winSizeX - len;
	}
}


void drawHzLine(int x1, int x2, int y, unsigned int color) {
	int len = x2 - x1;
	unsigned int* pixel = (unsigned int*)memory;
	pixel += x1 + y * winSizeX;
	std::fill_n(pixel, len, color);
}


void drawVertLine(int y1, int y2, int x) {
	unsigned int* pixel = (unsigned int*)memory;
	pixel += x + y1 * winSizeX;
	for (int a = y1; a < y2; a++) {
		*pixel = 0xffffff;
		pixel += winSizeX;
	}
}


void drawLine(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		swapInt(&x1, &x2);
		swapInt(&y1, &y2);
	}
	else if (y1 == y2) {
		drawHzLine(x1, x2, y1, 0xffffff);
		return;
	}
	else if (x1 == x2) {
		drawVertLine(y1, y2, x1);
		return;
	}

	unsigned int* pixel = (unsigned int*)memory;
	pixel += x1 + y1 * winSizeX;

	float m = (float)(y2 - y1) / (float)(x2 - x1);
	float x = 0.0f;

	if (m > 0) {
		while (y1 < y2) {
			while (x < 1.0f) {
				*pixel++ = 0xffffff;
				x += m;
			}
			while (x >= 1.0f) {
				pixel += winSizeX;
				y1++;
				if (y1 == y2) return;
				*pixel = 0xffffff;
				x -= 1.0f;
			}
		}
	}

	else {
		while (y1 > y2) {
			while (x < 1.0f) {
				*pixel++ = 0xffffff;
				x -= m;
			}
			while (x >= 1.0f) {
				pixel -= winSizeX;
				y1--;
				if (y1 == y2) return;
				*pixel = 0xffffff;
				x -= 1.0f;
			}
		}
	}
}


void drawVector(Vec3f vec) {
	Vec3f origin = { 0.0f, 0.0f, 0.0f };
	Vec2 originScreen = getScreenCoords(&origin);
	Vec2 vecScreen = getScreenCoords(&vec);
	originScreen.x = range(originScreen.x, 0, winSizeX - 1);
	originScreen.y = range(originScreen.y, 0, winSizeY - 1);
	vecScreen.x = range(vecScreen.x, 0, winSizeX - 1);
	vecScreen.y = range(vecScreen.y, 0, winSizeY - 1);
	drawLine(originScreen.x, originScreen.y, vecScreen.x, vecScreen.y);
}


void drawTriangleOutline(struct Tri2D tri) {
	drawLine(tri.pts[0].x, tri.pts[0].y, tri.pts[1].x, tri.pts[1].y);
	drawLine(tri.pts[1].x, tri.pts[1].y, tri.pts[2].x, tri.pts[2].y);
	drawLine(tri.pts[2].x, tri.pts[2].y, tri.pts[0].x, tri.pts[0].y);
}


void drawScanLine(Vec3f& scanLineStart, Vec3f& scanLineEnd, unsigned int color, int y) {
	int* pixel = (int*)memory;
	pixel += (int)scanLineStart.x + y * winSizeX;

	float* depth = (float*)zBuffer;
	depth += (int)scanLineStart.x + y * winSizeX;

	float thisDepth = scanLineStart.z;
	float thisDepthCh = (scanLineEnd.z - scanLineStart.z) / (scanLineEnd.x - scanLineStart.x);

	for (int b = (int)scanLineStart.x; b < scanLineEnd.x; b++) {
		if (thisDepth < *depth) {
			*pixel = color;
			*depth = thisDepth;
		}
		thisDepth += thisDepthCh;
		pixel++;
		depth++;
	}
}


void drawTriangle(Tri2D* tri, Tri3D* tri3D, unsigned int color) {
	sortTriToRaster(tri, tri3D);

	Line2D edges[3] = {
		Line2D(tri->pts[0], tri->pts[1]),
		Line2D(tri->pts[1], tri->pts[2]),
		Line2D(tri->pts[2], tri->pts[0])
	};

	float triDepth[3] = {
		getMagnitudeVec3(tri3D->pts[0]),
		getMagnitudeVec3(tri3D->pts[1]),
		getMagnitudeVec3(tri3D->pts[2])
	};

	Vec3f scanLineStart = {
		(float)tri->pts[2].x,
		0.0f,    // y not used
		triDepth[2]
	};
	Vec3f scanLineEnd = scanLineStart;

	if (tri->pts[1].x > edges[2].getXatY(tri->pts[1].y)) {
		float depthCh1 = (triDepth[2] - triDepth[0]) / (tri->pts[2].y - tri->pts[0].y);
		float depthCh2 = (triDepth[2] - triDepth[1]) / (tri->pts[2].y - tri->pts[1].y);

		for (int a = tri->pts[2].y; a > tri->pts[1].y; a--) {
			scanLineStart.x -= edges[2].invSlope;
			scanLineEnd.x -= edges[1].invSlope;

			scanLineStart.z -= depthCh1;
			scanLineEnd.z -= depthCh2;

			drawScanLine(scanLineStart, scanLineEnd, color, a);
		}

		scanLineStart.x = (float)edges[2].getXatY(tri->pts[1].y);
		scanLineEnd.x = (float)tri->pts[1].x;

		depthCh2 = (triDepth[1] - triDepth[0]) / (tri->pts[1].y - tri->pts[0].y);

		for (int a = tri->pts[1].y; a > tri->pts[0].y; a--) {
			scanLineStart.x -= edges[2].invSlope;
			scanLineEnd.x -= edges[0].invSlope;

			scanLineStart.z -= depthCh1;
			scanLineEnd.z -= depthCh2;

			drawScanLine(scanLineStart, scanLineEnd, color, a);
		}
	}
	
	else {
		float depthCh1 = (triDepth[2] - triDepth[1]) / (tri->pts[2].y - tri->pts[1].y);
		float depthCh2 = (triDepth[2] - triDepth[0]) / (tri->pts[2].y - tri->pts[0].y);
		
		for (int a = tri->pts[2].y; a > tri->pts[1].y; a--) {
			scanLineStart.x -= edges[1].invSlope;
			scanLineEnd.x -= edges[2].invSlope;

			scanLineStart.z -= depthCh1;
			scanLineEnd.z -= depthCh2;

			drawScanLine(scanLineStart, scanLineEnd, color, a);
		}

		scanLineStart.x = (float)tri->pts[1].x;
		scanLineEnd.x = (float)edges[2].getXatY(tri->pts[1].y);

		depthCh1 = (triDepth[1] - triDepth[0]) / (tri->pts[1].y - tri->pts[0].y);

		for (int a = tri->pts[1].y; a > tri->pts[0].y; a--) {
			scanLineStart.x -= edges[0].invSlope;
			scanLineEnd.x -= edges[2].invSlope;

			scanLineStart.z -= depthCh1;
			scanLineEnd.z -= depthCh2;

			drawScanLine(scanLineStart, scanLineEnd, color, a);
		}
	}
}
