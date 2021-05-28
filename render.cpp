#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "global.h"
#include "render.h"
#include "draw.h"
#include "util.h"
#include "clip.h"


Camera cam = {
	{ 0.0f, 0.0f, -4.0f }, // pos (world units)
	{ 0.0f, 0.0f, 0.0f }   // rot (degrees)
};

Vec3f lightVec = { 0.0f, 0.0f, -6.0f };


class Object3D {
private:
	std::vector<Triangle> triangles;

public:
	Object3D(std::vector<Tri3D> &aTris) {
		for (int a = 0; a < (int)aTris.size(); a++) {
			triangles.push_back(aTris[a]);
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
			// clipping
			tri.calcTriScreen();
			std::vector<Tri2D> clippedTris;
			clipTriangle(clippedTris, tri.triScreen);

			// display clipped tris
			for (auto& thisTri : clippedTris) {
				drawTriangleOutline(thisTri);
			}
		}
	}

	void drawObject() {
		calcAllTriCamera();

		// sort triangles based on distance from camera
		std::sort(triangles.begin(), triangles.end(), 
			[](Triangle& tri1, Triangle& tri2) {
				float z1 = (tri1.triCamera.pts[0].z + tri1.triCamera.pts[1].z + tri1.triCamera.pts[2].z) / 3.0f;
				float z2 = (tri2.triCamera.pts[0].z + tri2.triCamera.pts[1].z + tri2.triCamera.pts[2].z) / 3.0f;
				return z1 > z2;
			}
		);

		// draw all triangles
		for (auto& tri : triangles) {
			Vec3f normalCam = tri.calcNormal(tri.triCamera);
			Vec3f normalWorld = tri.calcNormal(tri.triWorld);

			if (dotProductVec3(tri.triCamera.pts[0], normalCam) < 0.0f) {
				// color/shading
				float sim = similarityVec3(normalWorld, lightVec);
				Vec3f rgb = { 255.0f, 255.0f, 255.0f };
				rgb = scalarMultVec3(rgb, sim);
				unsigned int color = rgbToDec((int)rgb.x, (int)rgb.y, (int)rgb.z);

				// clipping
				tri.calcTriScreen();
				std::vector<Tri2D> clippedTris;
				clipTriangle(clippedTris, tri.triScreen);

				// draw clipped tris
				for (auto& thisTri : clippedTris) {
					drawTriangle(&thisTri, &tri.triCamera, color);
				}
			}
		}
	}
};


std::vector<Object3D> objs;


void loadObjFile(const char* fileName) {
	std::vector<Vec3f> points;
	std::vector<Tri3D> triangles;

	std::string ltr = "";
	std::ifstream file(fileName);

	while (file >> ltr) {
		// vertex data
		if (ltr == "v") {
			Vec3f tempPt;
			file >> tempPt.x >> tempPt.y >> tempPt.z;
			points.push_back(tempPt);
		}

		// triangle data
		else if (ltr == "f") {
			Tri3D tempTri;
			for (int a = 0; a < 3; a++) {
				std::string nums = "";
				std::stringstream stream;
				int index;

				file >> nums;
				stream << nums;
				stream >> index;
				tempTri.pts[a] = points[index - 1];
			}
			triangles.push_back(tempTri);
		}
	}

	file.close();
	objs.push_back(triangles);
}


void renderMain() {
	cam.calcTrigValues();

	for (auto& thisObj : objs) {
		//thisObj.rotateObject(dTime * 0.2f, dTime * 0.5f, dTime * 0.8f);
		thisObj.drawObject();
	}
}


void renderStart() {
	/*
	for (int a = 0; a < 8; a++) {
		loadObjFile("cube.obj");
	}

	objs[0].translateObject(-1.0f, -1.0f, -1.0f);
	objs[1].translateObject(-1.0f, -1.0f,  1.0f);
	objs[2].translateObject(-1.0f,  1.0f, -1.0f);
	objs[3].translateObject(-1.0f,  1.0f,  1.0f);
	objs[4].translateObject( 1.0f, -1.0f, -1.0f);
	objs[5].translateObject( 1.0f, -1.0f,  1.0f);
	objs[6].translateObject( 1.0f,  1.0f, -1.0f);
	objs[7].translateObject( 1.0f,  1.0f,  1.0f);
	*/

	loadObjFile("amogus.obj");
}
