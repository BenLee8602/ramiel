#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "util.h"
#include "global.h"
#include "vec.h"
#include "camera.h"


struct Tri2D {
	struct Vec2 pts[3];
};

struct Tri3D {
	struct Vec3f pts[3];
};


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


#endif
