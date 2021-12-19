#include <algorithm>
#include "triangle.h"
#include "render.h"

namespace bl {

	Triangle::Triangle(Vertex** pts) {
		this->pts[0] = pts[0];
		this->pts[1] = pts[1];
		this->pts[2] = pts[2];
	}


	void Triangle::draw_f(const Vec3f& color_e) const {
		// conv from world-space to camera-space
		Vec3f triCam[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCam[a] = RenderBL::cam.getCameraCoord(pts[a]->pos);
		}

		// only draw tris facing camera
		Vec3f normalCam = crossProduct(triCam[1] - triCam[0], triCam[2] - triCam[0]);
		if (dotProduct(triCam[0], normalCam) < 0.0f) {
			if (clip_f(triCam, color_e)) raster_f(triCam, color_e);
		}
	}


	void Triangle::draw_v() const {
		// conv from world-space to camera-space
		Vec3f triCam[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCam[a] = RenderBL::cam.getCameraCoord(pts[a]->pos);
		}

		// only draw tris facing camera
		Vec3f normalCam = crossProduct(triCam[1] - triCam[0], triCam[2] - triCam[0]);
		if (dotProduct(triCam[0], normalCam) < 0.0f) {
			Vec3f tri_c[3] = { pts[0]->color, pts[1]->color, pts[2]->color }; // vertex colors
			if (clip_v(triCam, tri_c)) raster_v(triCam, tri_c);
		}
	}


	void Triangle::draw_p(const Vec3f& color_e) const {
		// conv from world-space to camera-space
		Vec3f triCam[3] = { 0.0f };
		for (int a = 0; a < 3; a++) {
			triCam[a] = RenderBL::cam.getCameraCoord(pts[a]->pos);
		}

		// only draw tris facing camera
		Vec3f normalCam = crossProduct(triCam[1] - triCam[0], triCam[2] - triCam[0]);
		if (dotProduct(triCam[0], normalCam) < 0.0f) {
			Vertex tri_c[3] = { *pts[0], *pts[1], *pts[2] };
			if (clip_p(triCam, tri_c, color_e)) raster_p(triCam, tri_c, color_e);
		}
	}


	Vec3f Triangle::getNormal() const {
		Vec3f vec1 = pts[1]->pos - pts[0]->pos;
		Vec3f vec2 = pts[2]->pos - pts[0]->pos;
		return getNormalized(crossProduct(vec1, vec2));
	}


	Vertex* Triangle::operator[](int index) const {
		return pts[index];
	}


	bool Triangle::clip_f(Vec3f* tri, const Vec3f& color_e) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[1][Z]) / (tri[0][Z] - tri[1][Z]);
			float c2 = (RenderBL::znear - tri[1][Z]) / (tri[2][Z] - tri[1][Z]);

			// new tri formed from quad
			Vec3f temp[3] = { 0.0f };
			Vec3f temp_c[3] = { 0.0f };

			// interpolate x and y
			temp[0][X] = tri[1][X] + (tri[0][X] - tri[1][X]) * c1;
			temp[0][Y] = tri[1][Y] + (tri[0][Y] - tri[1][Y]) * c1;
			temp[0][Z] = RenderBL::znear;
			temp[1][X] = tri[1][X] + (tri[2][X] - tri[1][X]) * c2;
			temp[1][Y] = tri[1][Y] + (tri[2][Y] - tri[1][Y]) * c2;
			temp[1][Z] = RenderBL::znear;
			temp[2] = tri[2];

			// update original tri
			tri[1] = temp[0];

			// raster new tri
			raster_f(temp, color_e);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[0][Z]) / (tri[1][Z] - tri[0][Z]);
			float c2 = (RenderBL::znear - tri[2][Z]) / (tri[1][Z] - tri[2][Z]);

			// interpolate x and y
			tri[0][X] = tri[0][X] + (tri[1][X] - tri[0][X]) * c1;
			tri[0][Y] = tri[0][Y] + (tri[1][Y] - tri[0][Y]) * c1;
			tri[0][Z] = RenderBL::znear;
			tri[2][X] = tri[2][X] + (tri[1][X] - tri[2][X]) * c2;
			tri[2][Y] = tri[2][Y] + (tri[1][Y] - tri[2][Y]) * c2;
			tri[2][Z] = RenderBL::znear;
		};

		if (tri[0][Z] < RenderBL::znear) {
			if (tri[1][Z] < RenderBL::znear) {
				// case 2
				if (tri[2][Z] < RenderBL::znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tri[1], tri[2]);
					clip2();
				}
			}
			// case 8
			else if (tri[2][Z] < RenderBL::znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tri[1], tri[0]);
				clip1();
			}
		}

		else if (tri[1][Z] < RenderBL::znear) {
			// case 7
			if (tri[2][Z] < RenderBL::znear) {
				std::swap(tri[1], tri[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tri[2][Z] < RenderBL::znear) {
			std::swap(tri[1], tri[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	bool Triangle::clip_v(Vec3f* tri, Vec3f* tri_c) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[1][Z]) / (tri[0][Z] - tri[1][Z]);
			float c2 = (RenderBL::znear - tri[1][Z]) / (tri[2][Z] - tri[1][Z]);

			// new tri formed from quad
			Vec3f temp[3] = { 0.0f };
			Vec3f temp_c[3] = { 0.0f };

			// interpolate x and y
			temp[0][X] = tri[1][X] + (tri[0][X] - tri[1][X]) * c1;
			temp[0][Y] = tri[1][Y] + (tri[0][Y] - tri[1][Y]) * c1;
			temp[0][Z] = RenderBL::znear;
			temp[1][X] = tri[1][X] + (tri[2][X] - tri[1][X]) * c2;
			temp[1][Y] = tri[1][Y] + (tri[2][Y] - tri[1][Y]) * c2;
			temp[1][Z] = RenderBL::znear;
			temp[2] = tri[2];

			// interpolate color
			temp_c[0] = tri_c[1] + (tri_c[0] - tri_c[1]) * c1;
			temp_c[1] = tri_c[1] + (tri_c[2] - tri_c[1]) * c2;
			temp_c[2] = tri_c[2];

			// update original tri
			tri[1] = temp[0];
			tri_c[1] = temp_c[0];

			// raster new tri
			raster_v(temp, temp_c);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[0][Z]) / (tri[1][Z] - tri[0][Z]);
			float c2 = (RenderBL::znear - tri[2][Z]) / (tri[1][Z] - tri[2][Z]);

			// interpolate x and y
			tri[0][X] = tri[0][X] + (tri[1][X] - tri[0][X]) * c1;
			tri[0][Y] = tri[0][Y] + (tri[1][Y] - tri[0][Y]) * c1;
			tri[0][Z] = RenderBL::znear;
			tri[2][X] = tri[2][X] + (tri[1][X] - tri[2][X]) * c2;
			tri[2][Y] = tri[2][Y] + (tri[1][Y] - tri[2][Y]) * c2;
			tri[2][Z] = RenderBL::znear;

			// interpolate color
			tri_c[0] = tri_c[0] + (tri_c[1] - tri_c[0]) * c1;
			tri_c[2] = tri_c[2] + (tri_c[1] - tri_c[2]) * c2;
		};

		if (tri[0][Z] < RenderBL::znear) {
			if (tri[1][Z] < RenderBL::znear) {
				// case 2
				if (tri[2][Z] < RenderBL::znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tri[1], tri[2]);
					std::swap(tri_c[1], tri_c[2]);
					clip2();
				}
			}
			// case 8
			else if (tri[2][Z] < RenderBL::znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tri[1], tri[0]);
				std::swap(tri_c[1], tri_c[0]);
				clip1();
			}
		}

		else if (tri[1][Z] < RenderBL::znear) {
			// case 7
			if (tri[2][Z] < RenderBL::znear) {
				std::swap(tri[1], tri[0]);
				std::swap(tri_c[1], tri_c[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tri[2][Z] < RenderBL::znear) {
			std::swap(tri[1], tri[2]);
			std::swap(tri_c[1], tri_c[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	bool Triangle::clip_p(Vec3f* tri, Vertex* tri_c, const Vec3f& color_e) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[1][Z]) / (tri[0][Z] - tri[1][Z]);
			float c2 = (RenderBL::znear - tri[1][Z]) / (tri[2][Z] - tri[1][Z]);

			// new tri formed from quad
			Vec3f temp[3] = { 0.0f };
			Vertex temp_c[3] = { 0.0f };

			// interpolate x and y
			temp[0][X] = tri[1][X] + (tri[0][X] - tri[1][X]) * c1;
			temp[0][Y] = tri[1][Y] + (tri[0][Y] - tri[1][Y]) * c1;
			temp[0][Z] = RenderBL::znear;
			temp[1][X] = tri[1][X] + (tri[2][X] - tri[1][X]) * c2;
			temp[1][Y] = tri[1][Y] + (tri[2][Y] - tri[1][Y]) * c2;
			temp[1][Z] = RenderBL::znear;
			temp[2] = tri[2];

			// interpolate vertices
			temp_c[0].pos = tri_c[1].pos + (tri_c[0].pos - tri_c[1].pos) * c1;
			temp_c[1].pos = tri_c[1].pos + (tri_c[2].pos - tri_c[1].pos) * c2;
			temp_c[2].pos = tri_c[2].pos;

			temp_c[0].normal = tri_c[1].normal + (tri_c[0].normal - tri_c[1].normal) * c1;
			temp_c[1].normal = tri_c[1].normal + (tri_c[2].normal - tri_c[1].normal) * c2;
			temp_c[2].normal = tri_c[2].normal;

			// update original tri
			tri[1] = temp[0];
			tri_c[1] = temp_c[0];

			// raster new tri
			raster_p(temp, temp_c, color_e);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (RenderBL::znear - tri[0][Z]) / (tri[1][Z] - tri[0][Z]);
			float c2 = (RenderBL::znear - tri[2][Z]) / (tri[1][Z] - tri[2][Z]);

			// interpolate x and y
			tri[0][X] = tri[0][X] + (tri[1][X] - tri[0][X]) * c1;
			tri[0][Y] = tri[0][Y] + (tri[1][Y] - tri[0][Y]) * c1;
			tri[0][Z] = RenderBL::znear;
			tri[2][X] = tri[2][X] + (tri[1][X] - tri[2][X]) * c2;
			tri[2][Y] = tri[2][Y] + (tri[1][Y] - tri[2][Y]) * c2;
			tri[2][Z] = RenderBL::znear;

			// interpolate vertices
			tri_c[0].pos = tri_c[0].pos + (tri_c[1].pos - tri_c[0].pos) * c1;
			tri_c[2].pos = tri_c[2].pos + (tri_c[1].pos - tri_c[2].pos) * c2;

			tri_c[0].normal = tri_c[0].normal + (tri_c[1].normal - tri_c[0].normal) * c1;
			tri_c[2].normal = tri_c[2].normal + (tri_c[1].normal - tri_c[2].normal) * c2;
		};

		if (tri[0][Z] < RenderBL::znear) {
			if (tri[1][Z] < RenderBL::znear) {
				// case 2
				if (tri[2][Z] < RenderBL::znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tri[1], tri[2]);
					std::swap(tri_c[1], tri_c[2]);
					clip2();
				}
			}
			// case 8
			else if (tri[2][Z] < RenderBL::znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tri[1], tri[0]);
				std::swap(tri_c[1], tri_c[0]);
				clip1();
			}
		}

		else if (tri[1][Z] < RenderBL::znear) {
			// case 7
			if (tri[2][Z] < RenderBL::znear) {
				std::swap(tri[1], tri[0]);
				std::swap(tri_c[1], tri_c[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tri[2][Z] < RenderBL::znear) {
			std::swap(tri[1], tri[2]);
			std::swap(tri_c[1], tri_c[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	void Triangle::raster_f(Vec3f* tri, const Vec3f& color_e) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0][Y] > triScreen[1][Y]) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tri[0], tri[1]);
		}
		if (triScreen[0][Y] > triScreen[2][Y]) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tri[0], tri[2]);
		}
		if (triScreen[1][Y] > triScreen[2][Y]) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tri[1], tri[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2][Y] < 0 || triScreen[0][Y] > RenderBL::size[Y]) return;

		// get color
		Vec3f pos = (pts[0]->pos + pts[1]->pos + pts[2]->pos) / 3.0f;
		Vertex v = { pos, getNormal(), RenderBL::light_ambient };
		for (auto& l : RenderBL::lights) {
			l->getLight(v);
		}
		c_min(v.color);
		v.color -= color_e;
		c_max(v.color, RenderBL::light_ambient);
		int color = rgbToDec(v.color);

		// change in x per change in y
		float dx1_y = (float)(triScreen[2][X] - triScreen[0][X]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dx2_y = (float)(triScreen[1][X] - triScreen[0][X]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tri[2][Z] - tri[0][Z]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2_y = (tri[1][Z] - tri[0][Z]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// always ensure x1 < x2
		if (dx1_y > dx2_y) {
			std::swap(dx1_y, dx2_y);
			std::swap(dz1_y, dz2_y);
			dx_y = &dx1_y;
			dz_y = &dz1_y;
		}

		// y value of scanline
		int y = std::max(0, triScreen[0][Y]);
		int ymax = std::min(triScreen[1][Y], RenderBL::size[Y]);

		// start and end x values of scanline
		float x1 = (float)triScreen[0][X] + dx1_y * (float)(y - triScreen[0][Y]);
		float x2 = (float)triScreen[0][X] + dx2_y * (float)(y - triScreen[0][Y]);

		// start and end z values of scanline
		float z1 = tri[0][Z] + dz1_y * (float)(y - triScreen[0][Y]);
		float z2 = tri[0][Z] + dz2_y * (float)(y - triScreen[0][Y]);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, RenderBL::size[X]);

				float dz_x = (z2 - z1) / (x2 - x1);
				float z = z1 + dz_x * (x - (int)x1);

				int index = RenderBL::coordsToIndex({ x, y });

				for (x; x < xmax; x++) {
					if (z < RenderBL::depth[index]) {
						RenderBL::depth[index] = z;
						RenderBL::pixels_rgb[index] = v.color;
					}
					index++;
					z += dz_x;
				}
				x1 += dx1_y;
				x2 += dx2_y;

				z1 += dz1_y;
				z2 += dz2_y;
			}
		};

		// draw first tri segment
		rasterSegment();

		// update values for second segment
		ymax = std::min(triScreen[2][Y], RenderBL::size[Y]);

		*dx_y = (float)(triScreen[2][X]- triScreen[1][X]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		x1 = (float)triScreen[2][X] - dx1_y * (float)(triScreen[2][Y] - y);
		x2 = (float)triScreen[2][X] - dx2_y * (float)(triScreen[2][Y] - y);

		*dz_y = (tri[2][Z]- tri[1][Z]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1 = tri[2][Z] - dz1_y * (float)(triScreen[2][Y] - y);
		z2 = tri[2][Z] - dz2_y * (float)(triScreen[2][Y] - y);

		// draw second tri segment
		rasterSegment();
	}


	void Triangle::raster_v(Vec3f* tri, Vec3f* tri_c) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0][Y] > triScreen[1][Y]) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tri[0], tri[1]);
			std::swap(tri_c[0], tri_c[1]);
		}
		if (triScreen[0][Y] > triScreen[2][Y]) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tri[0], tri[2]);
			std::swap(tri_c[0], tri_c[2]);
		}
		if (triScreen[1][Y] > triScreen[2][Y]) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tri[1], tri[2]);
			std::swap(tri_c[1], tri_c[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2][Y] < 0 || triScreen[0][Y] > RenderBL::size[Y]) return;

		// change in x per change in y
		float dx1_y = (float)(triScreen[2][X] - triScreen[0][X]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dx2_y = (float)(triScreen[1][X] - triScreen[0][X]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tri[2][Z] - tri[0][Z]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2_y = (tri[1][Z] - tri[0][Z]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// change in color per change in y
		Vec3f dc1_y = (tri_c[2] - tri_c[0]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		Vec3f dc2_y = (tri_c[1] - tri_c[0]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		Vec3f* dc_y = &dc2_y;	// points to dc for side of triangle with pt1 (2 segments)

		// always ensure x1 < x2
		if (dx1_y > dx2_y) {
			std::swap(dx1_y, dx2_y);
			std::swap(dz1_y, dz2_y);
			std::swap(dc1_y, dc2_y);
			dx_y = &dx1_y;
			dz_y = &dz1_y;
			dc_y = &dc1_y;
		}

		// y value of scanline
		int y = std::max(0, triScreen[0][Y]);
		int ymax = std::min(triScreen[1][Y], RenderBL::size[Y]);

		// start and end x values of scanline
		float x1 = (float)triScreen[0][X] + dx1_y * (float)(y - triScreen[0][Y]);
		float x2 = (float)triScreen[0][X] + dx2_y * (float)(y - triScreen[0][Y]);

		// start and end z values of scanline
		float z1 = tri[0][Z] + dz1_y * (float)(y - triScreen[0][Y]);
		float z2 = tri[0][Z] + dz2_y * (float)(y - triScreen[0][Y]);

		// start and end color values of scanline
		Vec3f c1 = tri_c[0] + dc1_y * (float)(y - triScreen[0][Y]);
		Vec3f c2 = tri_c[0] + dc2_y * (float)(y - triScreen[0][Y]);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, RenderBL::size[X]);

				float dz_x = (z2 - z1) / (x2 - x1);
				float z = z1 + dz_x * (x - (int)x1);

				Vec3f dc_x = (c2 - c1) / (x2 - x1);
				Vec3f c = c1 + dc_x * (x - x1);

				int index = RenderBL::coordsToIndex({ x, y });

				for (x; x < xmax; x++) {
					if (z < RenderBL::depth[index]) {
						RenderBL::depth[index] = z;
						RenderBL::pixels_rgb[index] = c;
					}
					index++;
					z += dz_x;
					c += dc_x;
				}
				x1 += dx1_y;
				x2 += dx2_y;

				z1 += dz1_y;
				z2 += dz2_y;

				c1 += dc1_y;
				c2 += dc2_y;
			}
		};

		// draw first tri segment
		rasterSegment();

		// update values for second segment
		ymax = std::min(triScreen[2][Y], RenderBL::size[Y]);

		*dx_y = (float)(triScreen[2][X] - triScreen[1][X]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		x1 = (float)triScreen[2][X] - dx1_y * (float)(triScreen[2][Y] - y);
		x2 = (float)triScreen[2][X] - dx2_y * (float)(triScreen[2][Y] - y);

		*dz_y = (tri[2][Z] - tri[1][Z]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1 = tri[2][Z] - dz1_y * (float)(triScreen[2][Y] - y);
		z2 = tri[2][Z] - dz2_y * (float)(triScreen[2][Y] - y);

		*dc_y = (tri_c[2] - tri_c[1]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		c1 = tri_c[2] - dc1_y * (float)(triScreen[2][Y] - y);
		c2 = tri_c[2] - dc2_y * (float)(triScreen[2][Y] - y);

		// draw second tri segment
		rasterSegment();
	}


	void Triangle::raster_p(Vec3f* tri, Vertex* tri_c, const Vec3f& color_e) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = RenderBL::cam.getScreenCoord(tri[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0][Y] > triScreen[1][Y]) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tri[0], tri[1]);
			std::swap(tri_c[0], tri_c[1]);
		}
		if (triScreen[0][Y] > triScreen[2][Y]) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tri[0], tri[2]);
			std::swap(tri_c[0], tri_c[2]);
		}
		if (triScreen[1][Y] > triScreen[2][Y]) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tri[1], tri[2]);
			std::swap(tri_c[1], tri_c[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2][Y] < 0 || triScreen[0][Y] > RenderBL::size[Y]) return;

		// divide tri_c by z to account for perspective
		float tri_zinv[3] = { 1.0f / tri[0][Z], 1.0f / tri[1][Z], 1.0f / tri[2][Z] };
		for (int a = 0; a < 3; a++) {
			tri_c[a].pos *= tri_zinv[a];
			tri_c[a].normal *= tri_zinv[a];
		}

		// change in x per change in y
		float dx1_y = (float)(triScreen[2][X] - triScreen[0][X]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dx2_y = (float)(triScreen[1][X] - triScreen[0][X]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tri[2][Z] - tri[0][Z]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2_y = (tri[1][Z] - tri[0][Z]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// change in inverse z per change in y
		float dz1inv_y = (tri_zinv[2] - tri_zinv[0]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2inv_y = (tri_zinv[1] - tri_zinv[0]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dzinv_y = &dz2inv_y;
		
		// change in pos per change in y
		Vec3f dp1_y = (tri_c[2].pos - tri_c[0].pos) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		Vec3f dp2_y = (tri_c[1].pos - tri_c[0].pos) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		Vec3f* dp_y = &dp2_y;

		// change in normal per change in y
		Vec3f dn1_y = (tri_c[2].normal - tri_c[0].normal) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		Vec3f dn2_y = (tri_c[1].normal - tri_c[0].normal) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		Vec3f* dn_y = &dn2_y;

		// always ensure x1 < x2
		if (dx1_y > dx2_y) {
			std::swap(dx1_y, dx2_y);
			std::swap(dz1_y, dz2_y);
			std::swap(dz1inv_y, dz2inv_y);
			std::swap(dp1_y, dp2_y);
			std::swap(dn1_y, dn2_y);
			dx_y = &dx1_y;
			dz_y = &dz1_y;
			dzinv_y = &dz1inv_y;
			dp_y = &dp1_y;
			dn_y = &dn1_y;
		}

		// y value of scanline
		int y = std::max(0, triScreen[0][Y]);
		int ymax = std::min(triScreen[1][Y], RenderBL::size[Y]);

		// start and end x values of scanline
		float x1 = (float)triScreen[0][X] + dx1_y * (float)(y - triScreen[0][Y]);
		float x2 = (float)triScreen[0][X] + dx2_y * (float)(y - triScreen[0][Y]);

		// start and end z values of scanline
		float z1 = tri[0][Z] + dz1_y * (float)(y - triScreen[0][Y]);
		float z2 = tri[0][Z] + dz2_y * (float)(y - triScreen[0][Y]);

		// start and end inverse z values of scanline
		float z1inv = tri_zinv[0] + dz1inv_y * (float)(y - triScreen[0][Y]);
		float z2inv = tri_zinv[0] + dz2inv_y * (float)(y - triScreen[0][Y]);

		// start and end pos values of scanline
		Vec3f p1 = tri_c[0].pos + dp1_y * (float)(y - triScreen[0][Y]);
		Vec3f p2 = tri_c[0].pos + dp2_y * (float)(y - triScreen[0][Y]);

		// start and end normal values of scanline
		Vec3f n1 = tri_c[0].normal + dn1_y * (float)(y - triScreen[0][Y]);
		Vec3f n2 = tri_c[0].normal + dn2_y * (float)(y - triScreen[0][Y]);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, RenderBL::size[X]);

				float dz_x = (z2 - z1) / (x2 - x1);
				float z = z1 + dz_x * (x - (int)x1);

				float dzinv_x = (z2inv - z1inv) / (x2 - x1);
				float zinv = z1inv + dzinv_x * (x - (int)x1);

				Vec3f dp_x = (p2 - p1) / (x2 - x1);
				Vec3f p = p1 + dp_x * (x - (int)x1);

				Vec3f dn_x = (n2 - n1) / (x2 - x1);
				Vec3f n = n1 + dn_x * (x - (int)x1);

				int index = RenderBL::coordsToIndex({ x, y });

				for (x; x < xmax; x++) {
					if (z < RenderBL::depth[index]) {
						RenderBL::depth[index] = z;
						Vertex v = { p / zinv, getNormalized(n), RenderBL::light_ambient };
						for (auto& l : RenderBL::lights) {
							l->getLight(v);
						}
						c_min(v.color);
						v.color -= color_e;
						c_max(v.color, RenderBL::light_ambient);
						RenderBL::pixels_rgb[index] = v.color;// rgbToDec(v.color);
					}
					index++;
					z += dz_x;
					zinv += dzinv_x;
					p += dp_x;
					n += dn_x;
				}
				x1 += dx1_y;
				x2 += dx2_y;

				z1 += dz1_y;
				z2 += dz2_y;

				z1inv += dz1inv_y;
				z2inv += dz2inv_y;

				p1 += dp1_y;
				p2 += dp2_y;

				n1 += dn1_y;
				n2 += dn2_y;
			}
		};

		// draw first tri segment
		rasterSegment();

		// update values for second segment
		ymax = std::min(triScreen[2][Y], RenderBL::size[Y]);

		*dx_y = (float)(triScreen[2][X] - triScreen[1][X]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		x1 = (float)triScreen[2][X] - dx1_y * (float)(triScreen[2][Y] - y);
		x2 = (float)triScreen[2][X] - dx2_y * (float)(triScreen[2][Y] - y);

		*dz_y = (tri[2][Z] - tri[1][Z]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1 = tri[2][Z] - dz1_y * (float)(triScreen[2][Y] - y);
		z2 = tri[2][Z] - dz2_y * (float)(triScreen[2][Y] - y);

		*dzinv_y = (tri_zinv[2] - tri_zinv[1]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1inv = tri_zinv[2] - dz1inv_y * (float)(triScreen[2][Y] - y);
		z2inv = tri_zinv[2] - dz2inv_y * (float)(triScreen[2][Y] - y);

		*dp_y = (tri_c[2].pos - tri_c[1].pos) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		p1 = tri_c[2].pos - dp1_y * (float)(triScreen[2][Y] - y);
		p2 = tri_c[2].pos - dp2_y * (float)(triScreen[2][Y] - y);

		*dn_y = (tri_c[2].normal - tri_c[1].normal) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		n1 = tri_c[2].normal - dn1_y * (float)(triScreen[2][Y] - y);
		n2 = tri_c[2].normal - dn2_y * (float)(triScreen[2][Y] - y);

		// draw second tri segment
		rasterSegment();
	}

}
