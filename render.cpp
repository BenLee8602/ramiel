#include "global.h"
#include "render.h"
#include "draw.h"
using namespace std;

float zFar = 1000.0f;
struct Vec3f cameraPos = { 640.0f, 360.0f, 0.0f };
struct Vec3f cameraRot = { 0.0f };


struct Vec2 convDimension(struct Vec3* pt3D) {
	struct Vec2 pt2D = { 0 };
	float depth = (cameraPos.z + (float)pt3D->z) / zFar;
	pt2D.x = (pt3D->x * (1.0f - depth) + cameraPos.x * depth) - cameraPos.x + winMidX;
	pt2D.y = (pt3D->y * (1.0f - depth) + cameraPos.y * depth) - cameraPos.y + winMidY;
	return pt2D;
}


void renderMain() {
	zFar = cameraPos.z + 1000.0f;

	struct Vec3 pt1c = { 800, 200, 100 };
	struct Vec3 pt2c = { 900, 200, 100 };
	struct Vec3 pt3c = { 800, 100, 100 };
	struct Vec3 pt4c = { 900, 100, 100 };
	struct Vec3 pt5c = { 800, 200, 200 };
	struct Vec3 pt6c = { 900, 200, 200 };
	struct Vec3 pt7c = { 800, 100, 200 };
	struct Vec3 pt8c = { 900, 100, 200 };

	struct Tri3D cube[12] = {
		// front
		{ { pt3c, pt1c, pt2c } },
		{ { pt2c, pt4c, pt3c } },
		// right
		{ { pt4c, pt2c, pt6c } },
		{ { pt6c, pt8c, pt4c } },
		// back
		{ { pt8c, pt6c, pt5c } },
		{ { pt5c, pt7c, pt8c } },
		// left
		{ { pt7c, pt5c, pt1c } },
		{ { pt1c, pt3c, pt7c } },
		// top
		{ { pt1c, pt5c, pt6c } },
		{ { pt6c, pt2c, pt1c } },
		// bottom
		{ { pt4c, pt8c, pt7c } },
		{ { pt7c, pt3c, pt4c } }
	};

	struct Tri2D cube2[12] = { 0 };

	for (int a = 0; a < 12; a++) {
		for (int b = 0; b < 3; b++) {
			cube2[a].pts[b] = convDimension(&cube[a].pts[b]);
		}
		drawTriangle(&cube2[a]);
	}
}
