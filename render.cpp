#include "global.h"
#include "render.h"
#include "draw.h"
#include <math.h>

struct Vec3f cameraPos = { 0.0f, 0.0f, 0.0f };
struct Vec3f cameraRot = { 0.0f };


struct Vec2 getScreenCoords(struct Vec3* pt3D) {
	// adjust for camera position
	struct Vec3 pt3DTr = { 0 };
	pt3DTr.x = pt3D->x - cameraPos.x;
	pt3DTr.y = pt3D->y - cameraPos.y;
	pt3DTr.z = pt3D->z - cameraPos.z;

	// adjust for camera rotation
	struct Vec3 pt3DRot = { pt3DTr };
	pt3DRot.x = pt3DTr.x * cos(cameraRot.y) + pt3DTr.z * sin(cameraRot.y);
	pt3DRot.z = pt3DTr.x * -sin(cameraRot.y) + pt3DTr.z * cos(cameraRot.y);

	pt3DRot.y = pt3DRot.y * cos(cameraRot.x) + pt3DRot.z * -sin(cameraRot.x);
	pt3DRot.z = pt3DRot.y * sin(cameraRot.x) + pt3DRot.z * cos(cameraRot.x);

	// project to screen
	struct Vec2 pt2D = { 0 };
	if (pt3DRot.z != 0) {
		pt2D.x = pt3DRot.x * winSizeX / pt3DRot.z + (int)winMidX;
		pt2D.y = pt3DRot.y * winSizeX / pt3DRot.z + (int)winMidY;
	}

	return pt2D;
}


void renderMain() {
	// cube
	struct Vec3 pt1c = { -50, 50, 550 };
	struct Vec3 pt2c = { 50, 50, 550 };
	struct Vec3 pt3c = { -50, -50, 550 };
	struct Vec3 pt4c = { 50, -50, 550 };
	struct Vec3 pt5c = { -50, 50, 650 };
	struct Vec3 pt6c = { 50, 50, 650 };
	struct Vec3 pt7c = { -50, -50, 650 };
	struct Vec3 pt8c = { 50, -50, 650 };

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
			cube2[a].pts[b] = getScreenCoords(&cube[a].pts[b]);
		}
		drawTriangle(&cube2[a]);
	}
}
