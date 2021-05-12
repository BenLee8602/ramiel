#include <math.h>
#include <vector>

#include "global.h"
#include "render.h"
#include "draw.h"
#include "util.h"


class Camera cam = {
	{ 0.0f, 0.0f, -4.0f }, // pos (world units)
	{ 0.0f, 0.0f, 0.0f }   // rot (degrees)
};

struct Vec3f lightVec = { 0.0f, 0.0f, -6.0f };


class Triangle {
public:
	struct Tri3D triWorld;
	struct Tri3D triCamera;
	struct Tri2D triScreen;

	Triangle(Tri3D tri) {
		triWorld = tri;
		triCamera = { 0.0f };
		triScreen = { 0 };
	}

	void calcTriCamera() {
		for (int a = 0; a < 3; a++) {
			struct Vec3f in = { 0.0f };
			struct Vec3f out = { 0.0f };

			in.x = triWorld.pts[a].x;
			in.y = triWorld.pts[a].y;
			in.z = triWorld.pts[a].z;

			out.x = in.x - cam.pos.x;
			out.y = in.y - cam.pos.y;
			out.z = in.z - cam.pos.z;
			in = out;

			out.x = in.x * cam.cosz + in.y * -cam.sinz;
			out.y = in.x * cam.sinz + in.y * cam.cosz;
			in = out;

			out.x = in.x * cam.cosy + in.z * cam.siny;
			out.z = in.x * -cam.siny + in.z * cam.cosy;
			in = out;

			out.y = in.y * cam.cosx + in.z * -cam.sinx;
			out.z = in.y * cam.sinx + in.z * cam.cosx;

			triCamera.pts[a] = out;
		}
	}

	void calcTriScreen() {
		for (int a = 0; a < 3; a++) {
			struct Vec2 screenPt = { 0 };
			if (triCamera.pts[a].z != 0) {
				screenPt.x = (int)(triCamera.pts[a].x * winSizeX / triCamera.pts[a].z + winMidX);
				screenPt.y = (int)(triCamera.pts[a].y * winSizeX / triCamera.pts[a].z + winMidY);
			}
			triScreen.pts[a] = screenPt;
		}
	}

	struct Vec3f calcNormal(Tri3D tri) {
		struct Vec3f aVec = subtractVec3(tri.pts[1], tri.pts[0]);
		struct Vec3f bVec = subtractVec3(tri.pts[2], tri.pts[0]);
		return crossProductVec3(aVec, bVec);
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

	void translateObject(float xChange, float yChange, float zChange) {
		for (auto& tri : triangles) {
			for (int a = 0; a < 3; a++) {
				tri.triWorld.pts[a].x += xChange;
				tri.triWorld.pts[a].y += yChange;
				tri.triWorld.pts[a].z += zChange;
			}
		}
	}

	void rotateObject(float xChange, float yChange, float zChange) {
		// rotates about origin
		for (auto& tri : triangles) {
			for (int a = 0; a < 3; a++) {
				struct Vec3f in = tri.triWorld.pts[a];
				struct Vec3f out = in;

				out.x = in.x * cosf(zChange) + in.y * -sinf(zChange);
				out.y = in.x * sinf(zChange) + in.y * cosf(zChange);
				in = out;

				out.x = in.x * cosf(yChange) + in.z * sinf(yChange);
				out.z = in.x * -sinf(yChange) + in.z * cosf(yChange);
				in = out;

				out.y = in.y * cosf(xChange) + in.z * -sinf(xChange);
				out.z = in.y * sinf(xChange) + in.z * cosf(xChange);

				tri.triWorld.pts[a] = out;
			}
		}
	}

	void calcAllTriCamera() {
		for (auto& tri : triangles) {
			tri.calcTriCamera();
		}
	}

	void calcAllTriScreen() {
		for (auto& tri : triangles) {
			tri.calcTriScreen();
		}
	}

	void drawWireMesh() {
		for (auto& tri : triangles) {
			if (dotProductVec3(tri.triCamera.pts[0], tri.calcNormal(tri.triCamera)) <= 0.0f) {
				drawTriangleOutline(&tri.triScreen);
			}
		}
	}

	void drawObject() {
		for (auto& tri : triangles) {
			Vec3f normalCam = tri.calcNormal(tri.triCamera);
			Vec3f normalWorld = tri.calcNormal(tri.triWorld);

			if (dotProductVec3(tri.triCamera.pts[0], normalCam) < 0.0f) {
				float sim = similarityVec3(normalWorld, lightVec);

				Vec3f rgb = { 255.0f, 255.0f, 255.0f };
				rgb = scalarMultVec3(rgb, sim);
				unsigned int color = rgbToDec(rgb.x, rgb.y, rgb.z);

				tri.calcTriScreen();
				drawTriangle(&tri.triScreen, color);
			}
		}
	}
};


struct Vec2 getScreenCoords(struct Vec3f* pt3D) {
	struct Vec3f in = { 0.0f };
	struct Vec3f out = { 0.0f };
	
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

	objs.at(0).rotateObject(dTime * 0.8f, dTime * 1.0f, dTime * 1.2f);

	for (auto& thisObj : objs) {
		thisObj.calcAllTriCamera();
		thisObj.drawObject();
	}
}


void renderStart() {
	struct Tri3D cube[12] = {
		// front
		{ { { -0.5f, -0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f }, {  0.5f,  0.5f, -0.5f } } },
		{ { {  0.5f,  0.5f, -0.5f }, {  0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f } } },
		// right
		{ { {  0.5f, -0.5f, -0.5f }, {  0.5f,  0.5f, -0.5f }, {  0.5f,  0.5f,  0.5f } } },
		{ { {  0.5f,  0.5f,  0.5f }, {  0.5f, -0.5f,  0.5f }, {  0.5f, -0.5f, -0.5f } } },
		// back
		{ { {  0.5f, -0.5f,  0.5f }, {  0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f } } },
		{ { { -0.5f,  0.5f,  0.5f }, { -0.5f, -0.5f,  0.5f }, {  0.5f, -0.5f,  0.5f } } },
		// left
		{ { { -0.5f, -0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f, -0.5f } } },
		{ { { -0.5f,  0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f,  0.5f } } },
		// top
		{ { { -0.5f,  0.5f, -0.5f }, { -0.5f,  0.5f,  0.5f }, {  0.5f,  0.5f,  0.5f } } },
		{ { {  0.5f,  0.5f,  0.5f }, {  0.5f,  0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f } } },
		// bottom
		{ { {  0.5f, -0.5f, -0.5f }, {  0.5f, -0.5f,  0.5f }, { -0.5f, -0.5f,  0.5f } } },
		{ { { -0.5f, -0.5f,  0.5f }, { -0.5f, -0.5f, -0.5f }, {  0.5f, -0.5f, -0.5f } } }
	};

	objs.push_back(Object3D(12, cube));
}
