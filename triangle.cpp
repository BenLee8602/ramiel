#include <algorithm>
#include "triangle.h"
#include "render.h"

namespace bl {

	Vec3f Triangle::getNormal(const Vec3f* tri) const {
		return crossProduct(tri[1] - tri[0], tri[2] - tri[0]);
	}


	Triangle::Triangle() {
		pts[0] = { 0.0f };
		pts[1] = { 0.0f };
		pts[2] = { 0.0f };
	}


	Triangle::Triangle(Vec3f& pt0, Vec3f& pt1, Vec3f& pt2) {
		pts[0] = pt0;
		pts[1] = pt1;
		pts[2] = pt2;
	}

	
	void Triangle::draw() const {
		// conv from world-space to camera-space
		Vec3f triCamera[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCamera[a] = RenderBL::cam.getCameraCoord(pts[a]);
		}

		// get normals
		Vec3f normalWorld = getNormal(pts);
		Vec3f normalCam = getNormal(triCamera);

		// only draw tris facing camera
		if (dotProduct(triCamera[0], normalCam) < 0.0f) {
			// get color
			Vec3f rgb = { 255, 255, 255 };
			float sim = dirSimilarity(normalWorld, { 0.0f, 0.0f, -1.0f });
			rgb *= sim;
			int color = ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;

			// project tri onto screen
			Vec2 triScreen[3] = { 0 };
			for (int a = 0; a < 3; a++) {
				triScreen[a] = RenderBL::cam.getScreenCoord(triCamera[a]);
			}

			// sort tri pts by y ascending
			if (triScreen[0].y > triScreen[1].y) std::swap(triScreen[0], triScreen[1]);
			if (triScreen[0].y > triScreen[2].y) std::swap(triScreen[0], triScreen[2]);
			if (triScreen[1].y > triScreen[2].y) std::swap(triScreen[1], triScreen[2]);

			// start and end x values of scanline
			float x1 = (float)triScreen[0].x;
			float x2 = (float)triScreen[0].x;

			// change in x per change in y
			float dx1 = (float)(triScreen[2].x - triScreen[0].x) / (float)(triScreen[2].y - triScreen[0].y);
			float dx2 = (float)(triScreen[1].x - triScreen[0].x) / (float)(triScreen[1].y - triScreen[0].y);

			float* dx = &dx2;	// points to dx for side of triangle with pt1 (2 segments)
			int index;			// index of pixel buffer to be written to

			// always ensure x1 < x2
			if (dx1 > dx2) {
				std::swap(dx1, dx2);
				dx = &dx1;
			}
			
			// draw triangle from triScreen[0].y to triScreen[1].y
			for (int y = triScreen[0].y; y < triScreen[1].y; y++) {
				index = RenderBL::coordsToIndex({ (int)x1, y });
				for (float x = x1; x < x2; x += 1.0f) {
					RenderBL::pixels[index++] = color;
				}
				x1 += dx1;
				x2 += dx2;
			}

			// update dx for second segment
			*dx = (float)(triScreen[2].x - triScreen[1].x) / (float)(triScreen[2].y - triScreen[1].y);
			x1 = (float)triScreen[2].x - (float)(triScreen[2].y - triScreen[1].y) * dx1;
			x2 = (float)triScreen[2].x - (float)(triScreen[2].y - triScreen[1].y) * dx2;
			
			// draw triangle from triScreen[1].y to triScreen[2].y
			for (int y = triScreen[1].y; y < triScreen[2].y; y++) {
				index = RenderBL::coordsToIndex({ (int)x1, y });
				for (float x = x1; x < x2; x += 1.0f) {
					RenderBL::pixels[index++] = color;
				}
				x1 += dx1;
				x2 += dx2;
			}
		}
	}

}
