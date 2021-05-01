#include <math.h>
#include <vector>

#include "global.h"
#include "render.h"
#include "draw.h"


class Camera cam = { };


class Triangle {
public:
	struct Tri3D triWorld;
	struct Tri2D triScreen;

	Triangle(Tri3D tri) {
		triWorld = tri;
		triScreen = { 0 };
	}

	void calcTriScreen() {
		for (int a = 0; a < 3; a++) {
			triScreen.pts[a] = getScreenCoords(&triWorld.pts[a]);
		}
	}
};


class Object3D {
private:
	std::vector<Triangle> triangles;

public:
	Object3D(int numTris, Tri3D* tris) {
		for (int a = 0; a < numTris; a++) {
			triangles.push_back(Triangle(*tris));
			tris++;
		}
	}

	void translateObject(int xChange, int yChange, int zChange) {
		for (auto &element : triangles) {
			for (int a = 0; a < 3; a++) {
				element.triWorld.pts[a].x += xChange;
				element.triWorld.pts[a].y += yChange;
				element.triWorld.pts[a].z += zChange;
			}
		}
	}

	void drawWireMesh() {
		for (auto &element : triangles) {
			element.calcTriScreen();
			drawTriangle(&element.triScreen);
		}
	}
};


struct Vec2 getScreenCoords(struct Vec3* pt3D) {
	struct Vec3f in = { 0.0f };
	struct Vec3f out = { 0.0f };
	
	in.x = (float)pt3D->x;
	in.y = (float)pt3D->y;
	in.z = (float)pt3D->z;

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
	struct Vec2 screenPt = { 0 };
	if (out.z != 0) {
		screenPt.x = (int)(out.x * winSizeX / out.z + winMidX);
		screenPt.y = (int)(out.y * winSizeX / out.z + winMidY);
	}

	return screenPt;
}


std::vector<Object3D> objs;


void renderMain() {
	cam.calcTrigValues();

	for (auto thisObj : objs) {
		thisObj.drawWireMesh();
	}
}


void renderStart() {
	struct Tri3D cubeTri[12] = {
		// front
		{ { { -1, -1, -1 }, { -1,  1, -1 }, {  1,  1, -1 } } },
		{ { {  1,  1, -1 }, {  1, -1, -1 }, { -1, -1, -1 } } },
		// right
		{ { {  1, -1, -1 }, {  1,  1, -1 }, {  1,  1,  1 } } },
		{ { {  1,  1,  1 }, {  1, -1,  1 }, {  1, -1, -1 } } },
		// back
		{ { {  1, -1,  1 }, {  1,  1,  1 }, { -1,  1,  1 } } },
		{ { { -1,  1,  1 }, { -1, -1,  1 }, {  1, -1,  1 } } },
		// left
		{ { { -1, -1,  1 }, { -1,  1,  1 }, { -1,  1, -1 } } },
		{ { { -1,  1, -1 }, { -1, -1, -1 }, { -1, -1,  1 } } },
		// top
		{ { { -1,  1, -1 }, { -1,  1,  1 }, {  1,  1,  1 } } },
		{ { {  1,  1,  1 }, {  1,  1, -1 }, { -1,  1, -1 } } },
		// bottom
		{ { {  1, -1, -1 }, {  1, -1,  1 }, { -1, -1,  1 } } },
		{ { { -1, -1,  1 }, { -1, -1, -1 }, {  1, -1, -1 } } }
	};

	objs.push_back(Object3D(12, cubeTri));
	objs.at(0).translateObject(0, 0, 10);
}
