#include <algorithm>
#include "triangle.h"
#include "render.h"

namespace bl {

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


	Vec3f Triangle::getNormal(const Vec3f* tri) const {
		return crossProduct(tri[1] - tri[0], tri[2] - tri[0]);
	}


	bool Triangle::clip(Vec3f* tri) const {
		if (tri[0].z < RenderBL::znear) {
			if (tri[1].z < RenderBL::znear) {
				// case 2
				if (tri[2].z < RenderBL::znear) {
					return false;
				}
				// case 6
				else {
					tri[0].x = tri[0].x + (tri[2].x - tri[0].x) * (RenderBL::znear - tri[0].z) / (tri[2].z - tri[0].z);
					tri[0].y = tri[0].y + (tri[2].y - tri[0].y) * (RenderBL::znear - tri[0].z) / (tri[2].z - tri[0].z);
					tri[0].z = RenderBL::znear;
					tri[1].x = tri[1].x + (tri[2].x - tri[1].x) * (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);
					tri[1].y = tri[1].y + (tri[2].y - tri[1].y) * (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);
					tri[1].z = RenderBL::znear;
				}
			}
			// case 8
			else if (tri[2].z < RenderBL::znear) {
				tri[0].x = tri[0].x + (tri[1].x - tri[0].x) * (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
				tri[0].y = tri[0].y + (tri[1].y - tri[0].y) * (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
				tri[0].z = RenderBL::znear;
				tri[2].x = tri[2].x + (tri[1].x - tri[2].x) * (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);
				tri[2].y = tri[2].y + (tri[1].y - tri[2].y) * (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);
				tri[2].z = RenderBL::znear;
			}
			// case 3
			else {
				Vec3f temp[3] = { 0.0f };
				temp[0].x = tri[0].x + (tri[2].x - tri[0].x) * (RenderBL::znear - tri[0].z) / (tri[2].z - tri[0].z);
				temp[0].y = tri[0].y + (tri[2].y - tri[0].y) * (RenderBL::znear - tri[0].z) / (tri[2].z - tri[0].z);
				temp[0].z = RenderBL::znear;
				temp[1].x = tri[0].x + (tri[1].x - tri[0].x) * (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
				temp[1].y = tri[0].y + (tri[1].y - tri[0].y) * (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
				temp[1].z = RenderBL::znear;
				temp[2] = tri[1];
				raster(temp);
				tri[0] = temp[0];
			}
		}

		else if (tri[1].z < RenderBL::znear) {
			// case 7
			if (tri[2].z < RenderBL::znear) {
				tri[1].x = tri[1].x + (tri[0].x - tri[1].x) * (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
				tri[1].y = tri[1].y + (tri[0].y - tri[1].y) * (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
				tri[1].z = RenderBL::znear;
				tri[2].x = tri[2].x + (tri[0].x - tri[2].x) * (RenderBL::znear - tri[2].z) / (tri[0].z - tri[2].z);
				tri[2].y = tri[2].y + (tri[0].y - tri[2].y) * (RenderBL::znear - tri[2].z) / (tri[0].z - tri[2].z);
				tri[2].z = RenderBL::znear;
			}
			// case 4
			else {
				Vec3f temp[3] = { 0.0f };
				temp[0].x = tri[1].x + (tri[0].x - tri[1].x) * (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
				temp[0].y = tri[1].y + (tri[0].y - tri[1].y) * (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
				temp[0].z = RenderBL::znear;
				temp[1].x = tri[1].x + (tri[2].x - tri[1].x) * (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);
				temp[1].y = tri[1].y + (tri[2].y - tri[1].y) * (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);
				temp[1].z = RenderBL::znear;
				temp[2] = tri[2];
				raster(temp);
				tri[1] = temp[0];
			}
		}

		// case 5
		else if (tri[2].z < RenderBL::znear) {
			Vec3f temp[3] = { 0.0f };
			temp[0].x = tri[2].x + (tri[0].x - tri[2].x) * (RenderBL::znear - tri[2].z) / (tri[0].z - tri[2].z);
			temp[0].y = tri[2].y + (tri[0].y - tri[2].y) * (RenderBL::znear - tri[2].z) / (tri[0].z - tri[2].z);
			temp[0].z = RenderBL::znear;
			temp[1].x = tri[2].x + (tri[1].x - tri[2].x) * (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);
			temp[1].y = tri[2].y + (tri[1].y - tri[2].y) * (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);
			temp[1].z = RenderBL::znear;
			temp[2] = tri[1];
			raster(temp);
			tri[2] = temp[0];
		}

		// case 1 (do nothing)

		return true;
	}


	void Triangle::raster(Vec3f* tri) const {
		// get color
		Vec3f normalWorld = getNormal(pts);
		Vec3f rgb = { 255, 255, 255 };
		float sim = dirSimilarity(normalWorld, { 0.0f, 0.0f, -1.0f });
		rgb *= sim;
		int color = ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;

		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0].y > triScreen[1].y) std::swap(triScreen[0], triScreen[1]);
		if (triScreen[0].y > triScreen[2].y) std::swap(triScreen[0], triScreen[2]);
		if (triScreen[1].y > triScreen[2].y) std::swap(triScreen[1], triScreen[2]);

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

		// y value of scanline
		int y = std::max(0, triScreen[0].y);
		int ymax = std::min(triScreen[1].y, RenderBL::size.y);

		// start and end x values of scanline
		float x1 = (float)triScreen[0].x + dx1 * (float)(y - triScreen[0].y);
		float x2 = (float)triScreen[0].x + dx2 * (float)(y - triScreen[0].y);;

		// draw first tri segment
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			index = RenderBL::coordsToIndex({ x, y });
			for (x; x < std::min((int)x2, RenderBL::size.x); x++) {
				RenderBL::pixels[index++] = color;
			}
			x1 += dx1;
			x2 += dx2;
		}

		// update values for second segment
		ymax = std::min(triScreen[2].y, RenderBL::size.y);
		*dx = (float)(triScreen[2].x - triScreen[1].x) / (float)(triScreen[2].y - triScreen[1].y);
		x1 = (float)triScreen[2].x - (float)(triScreen[2].y - y) * dx1;
		x2 = (float)triScreen[2].x - (float)(triScreen[2].y - y) * dx2;

		// draw second tri segment
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			index = RenderBL::coordsToIndex({ x, y });
			for (x; x < std::min((int)x2, RenderBL::size.x); x++) {
				RenderBL::pixels[index++] = color;
			}
			x1 += dx1;
			x2 += dx2;
		}
	}

	
	void Triangle::draw() const {
		// conv from world-space to camera-space
		Vec3f triCamera[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCamera[a] = RenderBL::cam.getCameraCoord(pts[a]);
		}

		// only draw tris facing camera
		Vec3f normalCam = getNormal(triCamera);
		if (dotProduct(triCamera[0], normalCam) < 0.0f) {
			if (clip(triCamera)) raster(triCamera);
		}
	}

}
