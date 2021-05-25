#include <math.h>
#include <fstream>
#include <string>
#include <sstream>

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
	Object3D(std::vector<Tri3D> &aTris) {
		for (int a = 0; a < aTris.size(); a++) {
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
		for (auto& tri : triangles) {
			Vec3f normalCam = tri.calcNormal(tri.triCamera);
			Vec3f normalWorld = tri.calcNormal(tri.triWorld);

			if (dotProductVec3(tri.triCamera.pts[0], normalCam) < 0.0f) {
				// color/shading
				float sim = similarityVec3(normalWorld, lightVec);
				Vec3f rgb = { 255.0f, 255.0f, 255.0f };
				rgb = scalarMultVec3(rgb, sim);
				unsigned int color = rgbToDec((int)rgb.x, rgb.y, rgb.z);

				// clipping
				tri.calcTriScreen();
				std::vector<Tri2D> clippedTris;
				clipTriangle(clippedTris, tri.triScreen);

				// display clipped tris
				for (auto& thisTri : clippedTris) {
					drawTriangle(thisTri, color);
				}
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


void clipTriangle(std::vector<Tri2D> &clippedTris, Tri2D triInput) {
	int xMin = 0;
	int yMin = 0;
	int xMax = winSizeX - 1;
	int yMax = winSizeY - 1;

	// convert input tri to edges
	std::vector<Line> edges;
	edges.push_back(Line(triInput.pts[0], triInput.pts[1]));
	edges.push_back(Line(triInput.pts[1], triInput.pts[2]));
	edges.push_back(Line(triInput.pts[2], triInput.pts[0]));

	// clip each line in triangle (x-axis)
	for (int a = 0; a < edges.size(); a++) {
		int lineState = edges[a].clipX(xMin, xMax);
		if (lineState == 5 || lineState == 10) {
			edges.erase(edges.begin() + a);
			a--;
		}
	}

	// return early if tri is not in frame
	if (edges.size() == 0) return;
	
	// connect gaps in lines with new lines
	connectLines(edges);

	// clip each line in triangle (y-axis)
	for (int a = 0; a < edges.size(); a++) {
		int lineState = edges[a].clipY(yMin, yMax);
		if (lineState == 5 || lineState == 10) {
			edges.erase(edges.begin() + a);
			a--;
		}
	}

	// return early if tri is not in frame
	if (edges.size() == 0) return;

	// connect gaps in lines with new lines
	connectLines(edges);
	
	// divide polygon into triangles
	for (int a = 0; a < edges.size() - 2; a++) {
		Tri2D newTri;
		newTri.pts[0] = edges[0].end;
		newTri.pts[1] = edges[a + 1].end;
		newTri.pts[2] = edges[a + 2].end;
		clippedTris.push_back(newTri);
	}
}


std::vector<Object3D> objs;


void loadObjFile(const char* fileName) {
	std::vector<Vec3f> points;
	std::vector<Tri3D> triangles;

	std::string ltr = "";
	std::ifstream file(fileName);

	while (file >> ltr) {
		if (ltr == "v") {
			Vec3f tempPt;
			file >> tempPt.x >> tempPt.y >> tempPt.z;
			points.push_back(tempPt);
		}

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
		thisObj.calcAllTriCamera();
		thisObj.drawObject();
	}
}


void renderStart() {/*
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
	objs[7].translateObject( 1.0f,  1.0f,  1.0f);*/
	loadObjFile("teapot.obj");
}
