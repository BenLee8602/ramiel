#include "global.h"
#include "render.h"
#include "draw.h"


void drawBackground() {
	unsigned int* pixel = (unsigned int*)bg;
	for (float y = 0.0f; y < winSizeY; y += 1) {
		for (float x = 0.0f; x < winSizeX; x += 1) {
			*pixel++ = 0;
		}
	}
}


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


void drawHzLine(int x1, int x2, int y) {
	unsigned int* pixel = (unsigned int*)memory;
	pixel += x1 + y * winSizeX;
	for (int a = x1; a < x2; a++) {
		*pixel++ = 0xffffff;
	}
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
		int temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	else if (y1 == y2) {
		drawHzLine(x1, x2, y1);
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
				*pixel = 0xffffff;
				x -= 1.0f;
			}
		}
	}
}


void drawTriangle(struct Tri2D* tri) {
	drawLine(tri->pts[0].x, tri->pts[0].y, tri->pts[1].x, tri->pts[1].y);
	drawLine(tri->pts[1].x, tri->pts[1].y, tri->pts[2].x, tri->pts[2].y);
	drawLine(tri->pts[2].x, tri->pts[2].y, tri->pts[0].x, tri->pts[0].y);
}
