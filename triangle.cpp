#include <algorithm>
#include "triangle.h"
#include "render.h"

namespace bl {

	Triangle::Triangle(Vertex** pts) {
		this->pts[0] = pts[0];
		this->pts[1] = pts[1];
		this->pts[2] = pts[2];
	}


	Vec3f Triangle::getNormal() const {
		return crossProduct(pts[1]->pos - pts[0]->pos, pts[2]->pos - pts[0]->pos).getNormalized();
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
					std::swap(tri[1], tri[2]);
					clip_2p(tri);
				}
			}
			// case 8
			else if (tri[2].z < RenderBL::znear) {
				clip_2p(tri);
			}
			// case 3
			else {
				std::swap(tri[1], tri[0]);
				clip_1p(tri);
			}
		}

		else if (tri[1].z < RenderBL::znear) {
			// case 7
			if (tri[2].z < RenderBL::znear) {
				std::swap(tri[1], tri[0]);
				clip_2p(tri);
			}
			// case 4
			else {
				clip_1p(tri);
			}
		}

		// case 5
		else if (tri[2].z < RenderBL::znear) {
			std::swap(tri[1], tri[2]);
			clip_1p(tri);
		}

		// case 1 (do nothing)

		return true;
	}

	void Triangle::clip_1p(Vec3f* tri) const {
		Vec3f temp[3] = { 0.0f };
		temp[0].x = tri[1].x + (tri[0].x - tri[1].x) * (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
		temp[0].y = tri[1].y + (tri[0].y - tri[1].y) * (RenderBL::znear - tri[1].z) / (tri[0].z - tri[1].z);
		temp[0].z = RenderBL::znear;
		temp[1].x = tri[1].x + (tri[2].x - tri[1].x) * (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);
		temp[1].y = tri[1].y + (tri[2].y - tri[1].y) * (RenderBL::znear - tri[1].z) / (tri[2].z - tri[1].z);
		temp[1].z = RenderBL::znear;
		temp[2] = tri[2];
		tri[1] = temp[0];
		raster(temp);
	}

	void Triangle::clip_2p(Vec3f* tri) const {
		tri[0].x = tri[0].x + (tri[1].x - tri[0].x) * (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
		tri[0].y = tri[0].y + (tri[1].y - tri[0].y) * (RenderBL::znear - tri[0].z) / (tri[1].z - tri[0].z);
		tri[0].z = RenderBL::znear;
		tri[2].x = tri[2].x + (tri[1].x - tri[2].x) * (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);
		tri[2].y = tri[2].y + (tri[1].y - tri[2].y) * (RenderBL::znear - tri[2].z) / (tri[1].z - tri[2].z);
		tri[2].z = RenderBL::znear;
	}


	void Triangle::raster(Vec3f* tri) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0].y > triScreen[1].y) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tri[0], tri[1]);
		}
		if (triScreen[0].y > triScreen[2].y) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tri[0], tri[2]);
		}
		if (triScreen[1].y > triScreen[2].y) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tri[1], tri[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2].y < 0 || triScreen[0].y > RenderBL::size.y) return;

		// get color
		Vec3f normalWorld = getNormal();
		Vec3f rgb = { 0.0f };
		for (auto& lgt : RenderBL::lights) {
			lgt.getLight(rgb, normalWorld);
		}
		int color = rgbToDec(rgb);

		// change in x per change in y
		float dx1_y = (float)(triScreen[2].x - triScreen[0].x) / (float)(triScreen[2].y - triScreen[0].y);
		float dx2_y = (float)(triScreen[1].x - triScreen[0].x) / (float)(triScreen[1].y - triScreen[0].y);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tri[2].z - tri[0].z) / (float)(triScreen[2].y - triScreen[0].y);
		float dz2_y = (tri[1].z - tri[0].z) / (float)(triScreen[1].y - triScreen[0].y);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// always ensure x1 < x2
		if (dx1_y > dx2_y) {
			std::swap(dx1_y, dx2_y);
			std::swap(dz1_y, dz2_y);
			dx_y = &dx1_y;
			dz_y = &dz1_y;
		}

		// y value of scanline
		int y = std::max(0, triScreen[0].y);
		int ymax = std::min(triScreen[1].y, RenderBL::size.y);

		// start and end x values of scanline
		float x1 = (float)triScreen[0].x + dx1_y * (float)(y - triScreen[0].y);
		float x2 = (float)triScreen[0].x + dx2_y * (float)(y - triScreen[0].y);

		// start and end z values of scanline
		float z1 = tri[0].z + dz1_y * (float)(y - triScreen[0].y);
		float z2 = tri[0].z + dz2_y * (float)(y - triScreen[0].y);

		// draw first tri segment
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			int xmax = std::min((int)x2, RenderBL::size.x);
			float dz_x = (z2 - z1) / (x2 - x1);
			float z = z1 + dz_x * (x - (int)x1);
			int index = RenderBL::coordsToIndex({ x, y });
			for (x; x < xmax; x++) {
				if (z < RenderBL::depth[index]) {
					RenderBL::depth[index] = z;
					RenderBL::pixels[index] = color;
				}
				index++;
				z += dz_x;
			}
			x1 += dx1_y;
			x2 += dx2_y;
			z1 += dz1_y;
			z2 += dz2_y;
		}

		// update values for second segment
		ymax = std::min(triScreen[2].y, RenderBL::size.y);

		*dx_y = (float)(triScreen[2].x - triScreen[1].x) / (float)(triScreen[2].y - triScreen[1].y);
		x1 = (float)triScreen[2].x - dx1_y * (float)(triScreen[2].y - y);
		x2 = (float)triScreen[2].x - dx2_y * (float)(triScreen[2].y - y);

		*dz_y = (tri[2].z - tri[1].z) / (float)(triScreen[2].y - triScreen[1].y);
		z1 = tri[2].z - dz1_y * (float)(triScreen[2].y - y);
		z2 = tri[2].z - dz2_y * (float)(triScreen[2].y - y);

		// draw second tri segment
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			int xmax = std::min((int)x2, RenderBL::size.x);
			float dz_x = (z2 - z1) / (x2 - x1);
			float z = z1 + dz_x * (x - (int)x1);
			int index = RenderBL::coordsToIndex({ x, y });
			for (x; x < xmax; x++) {
				if (z < RenderBL::depth[index]) {
					RenderBL::depth[index] = z;
					RenderBL::pixels[index] = color;
				}
				index++;
				z += dz_x;
			}
			x1 += dx1_y;
			x2 += dx2_y;
			z1 += dz1_y;
			z2 += dz2_y;
		}
	}

	
	void Triangle::draw() const {
		// conv from world-space to camera-space
		Vec3f triCam[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCam[a] = RenderBL::cam.getCameraCoord(pts[a]->pos);
		}

		// only draw tris facing camera
		Vec3f normalCam = crossProduct(triCam[1] - triCam[0], triCam[2] - triCam[0]);
		if (dotProduct(triCam[0], normalCam) < 0.0f) {
			if (clip(triCam)) raster(triCam);
		}
	}

	const Vertex* Triangle::operator[](int index) const {
		return pts[index];
	}

}
