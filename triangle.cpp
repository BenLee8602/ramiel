#include <algorithm>
#include "graphics.h"

namespace bl {

	Triangle::Triangle(size_t pts[3]) {
		this->pts[0] = pts[0];
		this->pts[1] = pts[1];
		this->pts[2] = pts[2];
	}


	void Triangle::draw_f(Vertex v[3], Vec3f tricam[3]) const {
		// only draw tris facing cameraera
		Vec3f normalcamera = crossProduct(tricam[1] - tricam[0], tricam[2] - tricam[0]);
		if (dotProduct(tricam[0], normalcamera) < 0.0f) {
			if (clip_f(v, tricam)) raster_f(v, tricam);
		}
	}


	void Triangle::draw_v(Vertex v[3], Vec3f tricam[3]) const {
		// only draw tris facing cameraera
		Vec3f normalcamera = crossProduct(tricam[1] - tricam[0], tricam[2] - tricam[0]);
		if (dotProduct(tricam[0], normalcamera) < 0.0f) {
			if (clip_v(v, tricam)) raster_v(v, tricam);
		}
	}


	void Triangle::draw_p(Vertex v[3], Vec3f tricam[3]) const {
		// only draw tris facing cameraera
		Vec3f normalcamera = crossProduct(tricam[1] - tricam[0], tricam[2] - tricam[0]);
		if (dotProduct(tricam[0], normalcamera) < 0.0f) {
			if (clip_p(v, tricam)) raster_p(v, tricam);
		}
	}


	size_t Triangle::operator[](size_t index) const {
		return pts[index];
	}


	bool Triangle::clip_f(Vertex v[3], Vec3f tricam[3]) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (GraphicsBL::camera.znear - tricam[1][Z]) / (tricam[0][Z] - tricam[1][Z]);
			float c2 = (GraphicsBL::camera.znear - tricam[1][Z]) / (tricam[2][Z] - tricam[1][Z]);

			// new tri formed from quad
			Vec3f tricam_temp[3] = { 0.0f };

			// interpolate x and y
			tricam_temp[0][X] = tricam[1][X] + (tricam[0][X] - tricam[1][X]) * c1;
			tricam_temp[0][Y] = tricam[1][Y] + (tricam[0][Y] - tricam[1][Y]) * c1;
			tricam_temp[0][Z] = GraphicsBL::camera.znear;
			tricam_temp[1][X] = tricam[1][X] + (tricam[2][X] - tricam[1][X]) * c2;
			tricam_temp[1][Y] = tricam[1][Y] + (tricam[2][Y] - tricam[1][Y]) * c2;
			tricam_temp[1][Z] = GraphicsBL::camera.znear;
			tricam_temp[2] = tricam[2];

			// update original tri
			tricam[1] = tricam_temp[0];

			// raster new tri
			raster_f(v, tricam_temp);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (GraphicsBL::camera.znear - tricam[0][Z]) / (tricam[1][Z] - tricam[0][Z]);
			float c2 = (GraphicsBL::camera.znear - tricam[2][Z]) / (tricam[1][Z] - tricam[2][Z]);

			// interpolate x and y
			tricam[0][X] = tricam[0][X] + (tricam[1][X] - tricam[0][X]) * c1;
			tricam[0][Y] = tricam[0][Y] + (tricam[1][Y] - tricam[0][Y]) * c1;
			tricam[0][Z] = GraphicsBL::camera.znear;
			tricam[2][X] = tricam[2][X] + (tricam[1][X] - tricam[2][X]) * c2;
			tricam[2][Y] = tricam[2][Y] + (tricam[1][Y] - tricam[2][Y]) * c2;
			tricam[2][Z] = GraphicsBL::camera.znear;
		};

		if (tricam[0][Z] < GraphicsBL::camera.znear) {
			if (tricam[1][Z] < GraphicsBL::camera.znear) {
				// case 2
				if (tricam[2][Z] < GraphicsBL::camera.znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tricam[1], tricam[2]);
					clip2();
				}
			}
			// case 8
			else if (tricam[2][Z] < GraphicsBL::camera.znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tricam[1], tricam[0]);
				clip1();
			}
		}

		else if (tricam[1][Z] < GraphicsBL::camera.znear) {
			// case 7
			if (tricam[2][Z] < GraphicsBL::camera.znear) {
				std::swap(tricam[1], tricam[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tricam[2][Z] < GraphicsBL::camera.znear) {
			std::swap(tricam[1], tricam[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	bool Triangle::clip_v(Vertex v[3], Vec3f tricam[3]) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (GraphicsBL::camera.znear - tricam[1][Z]) / (tricam[0][Z] - tricam[1][Z]);
			float c2 = (GraphicsBL::camera.znear - tricam[1][Z]) / (tricam[2][Z] - tricam[1][Z]);

			// new tri formed from quad
			Vec3f tricam_temp[3] = { 0.0f };
			Vertex v_temp[3] = { 0.0f };

			// interpolate x and y
			tricam_temp[0][X] = tricam[1][X] + (tricam[0][X] - tricam[1][X]) * c1;
			tricam_temp[0][Y] = tricam[1][Y] + (tricam[0][Y] - tricam[1][Y]) * c1;
			tricam_temp[0][Z] = GraphicsBL::camera.znear;
			tricam_temp[1][X] = tricam[1][X] + (tricam[2][X] - tricam[1][X]) * c2;
			tricam_temp[1][Y] = tricam[1][Y] + (tricam[2][Y] - tricam[1][Y]) * c2;
			tricam_temp[1][Z] = GraphicsBL::camera.znear;
			tricam_temp[2] = tricam[2];

			// interpolate color
			v_temp[0].color = v[1].color + (v[0].color - v[1].color) * c1;
			v_temp[1].color = v[1].color + (v[2].color - v[1].color) * c2;
			v_temp[2].color = v[2].color;

			// update original tri
			tricam[1] = tricam_temp[0];
			v[1] = v_temp[0];

			// raster new tri
			raster_v(v_temp, tricam_temp);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (GraphicsBL::camera.znear - tricam[0][Z]) / (tricam[1][Z] - tricam[0][Z]);
			float c2 = (GraphicsBL::camera.znear - tricam[2][Z]) / (tricam[1][Z] - tricam[2][Z]);

			// interpolate x and y
			tricam[0][X] = tricam[0][X] + (tricam[1][X] - tricam[0][X]) * c1;
			tricam[0][Y] = tricam[0][Y] + (tricam[1][Y] - tricam[0][Y]) * c1;
			tricam[0][Z] = GraphicsBL::camera.znear;
			tricam[2][X] = tricam[2][X] + (tricam[1][X] - tricam[2][X]) * c2;
			tricam[2][Y] = tricam[2][Y] + (tricam[1][Y] - tricam[2][Y]) * c2;
			tricam[2][Z] = GraphicsBL::camera.znear;

			// interpolate color
			v[0].color = v[0].color + (v[1].color - v[0].color) * c1;
			v[2].color = v[2].color + (v[1].color - v[2].color) * c2;
		};

		if (tricam[0][Z] < GraphicsBL::camera.znear) {
			if (tricam[1][Z] < GraphicsBL::camera.znear) {
				// case 2
				if (tricam[2][Z] < GraphicsBL::camera.znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tricam[1], tricam[2]);
					std::swap(v[1], v[2]);
					clip2();
				}
			}
			// case 8
			else if (tricam[2][Z] < GraphicsBL::camera.znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tricam[1], tricam[0]);
				std::swap(v[1], v[0]);
				clip1();
			}
		}

		else if (tricam[1][Z] < GraphicsBL::camera.znear) {
			// case 7
			if (tricam[2][Z] < GraphicsBL::camera.znear) {
				std::swap(tricam[1], tricam[0]);
				std::swap(v[1], v[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tricam[2][Z] < GraphicsBL::camera.znear) {
			std::swap(tricam[1], tricam[2]);
			std::swap(v[1], v[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	bool Triangle::clip_p(Vertex v[3], Vec3f tricam[3]) const {

		auto clip1 = [&]() {
			// ratio of line clipped
			float c1 = (GraphicsBL::camera.znear - tricam[1][Z]) / (tricam[0][Z] - tricam[1][Z]);
			float c2 = (GraphicsBL::camera.znear - tricam[1][Z]) / (tricam[2][Z] - tricam[1][Z]);

			// new tri formed from quad
			Vec3f tricam_temp[3] = { 0.0f };
			Vertex v_temp[3] = { 0.0f };

			// interpolate x and y
			tricam_temp[0][X] = tricam[1][X] + (tricam[0][X] - tricam[1][X]) * c1;
			tricam_temp[0][Y] = tricam[1][Y] + (tricam[0][Y] - tricam[1][Y]) * c1;
			tricam_temp[0][Z] = GraphicsBL::camera.znear;
			tricam_temp[1][X] = tricam[1][X] + (tricam[2][X] - tricam[1][X]) * c2;
			tricam_temp[1][Y] = tricam[1][Y] + (tricam[2][Y] - tricam[1][Y]) * c2;
			tricam_temp[1][Z] = GraphicsBL::camera.znear;
			tricam_temp[2] = tricam[2];

			// interpolate vertices
			v_temp[0].pos = v[1].pos + (v[0].pos - v[1].pos) * c1;
			v_temp[1].pos = v[1].pos + (v[2].pos - v[1].pos) * c2;
			v_temp[2].pos = v[2].pos;

			v_temp[0].normal = v[1].normal + (v[0].normal - v[1].normal) * c1;
			v_temp[1].normal = v[1].normal + (v[2].normal - v[1].normal) * c2;
			v_temp[2].normal = v[2].normal;

			// update original tri
			tricam[1] = tricam_temp[0];
			v[1] = v_temp[0];

			// raster new tri
			raster_p(v_temp, tricam_temp);
		};

		auto clip2 = [&]() {
			// ratio of line clipped
			float c1 = (GraphicsBL::camera.znear - tricam[0][Z]) / (tricam[1][Z] - tricam[0][Z]);
			float c2 = (GraphicsBL::camera.znear - tricam[2][Z]) / (tricam[1][Z] - tricam[2][Z]);

			// interpolate x and y
			tricam[0][X] = tricam[0][X] + (tricam[1][X] - tricam[0][X]) * c1;
			tricam[0][Y] = tricam[0][Y] + (tricam[1][Y] - tricam[0][Y]) * c1;
			tricam[0][Z] = GraphicsBL::camera.znear;
			tricam[2][X] = tricam[2][X] + (tricam[1][X] - tricam[2][X]) * c2;
			tricam[2][Y] = tricam[2][Y] + (tricam[1][Y] - tricam[2][Y]) * c2;
			tricam[2][Z] = GraphicsBL::camera.znear;

			// interpolate vertices
			v[0].pos = v[0].pos + (v[1].pos - v[0].pos) * c1;
			v[2].pos = v[2].pos + (v[1].pos - v[2].pos) * c2;

			v[0].normal = v[0].normal + (v[1].normal - v[0].normal) * c1;
			v[2].normal = v[2].normal + (v[1].normal - v[2].normal) * c2;
		};

		if (tricam[0][Z] < GraphicsBL::camera.znear) {
			if (tricam[1][Z] < GraphicsBL::camera.znear) {
				// case 2
				if (tricam[2][Z] < GraphicsBL::camera.znear) {
					return false;
				}
				// case 6
				else {
					std::swap(tricam[1], tricam[2]);
					std::swap(v[1], v[2]);
					clip2();
				}
			}
			// case 8
			else if (tricam[2][Z] < GraphicsBL::camera.znear) {
				clip2();
			}
			// case 3
			else {
				std::swap(tricam[1], tricam[0]);
				std::swap(v[1], v[0]);
				clip1();
			}
		}

		else if (tricam[1][Z] < GraphicsBL::camera.znear) {
			// case 7
			if (tricam[2][Z] < GraphicsBL::camera.znear) {
				std::swap(tricam[1], tricam[0]);
				std::swap(v[1], v[0]);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (tricam[2][Z] < GraphicsBL::camera.znear) {
			std::swap(tricam[1], tricam[2]);
			std::swap(v[1], v[2]);
			clip1();
		}

		// case 1 (do nothing)

		return true;
	}


	void Triangle::raster_f(Vertex v[3], Vec3f tricam[3]) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = GraphicsBL::camera.getScreenCoord(tricam[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0][Y] > triScreen[1][Y]) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tricam[0], tricam[1]);
		}
		if (triScreen[0][Y] > triScreen[2][Y]) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tricam[0], tricam[2]);
		}
		if (triScreen[1][Y] > triScreen[2][Y]) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tricam[1], tricam[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2][Y] < 0 || triScreen[0][Y] > GraphicsBL::size[Y]) return;

		// get color
		Vec3f pos = (v[0].pos + v[1].pos + v[2].pos) / 3.0f;
		Vec3f n = getNormalized(crossProduct(v[1].pos - v[0].pos, v[2].pos - v[0].pos));
		Vertex c = { pos, n, GraphicsBL::light_ambient };
		for (auto& l : GraphicsBL::lights) {
			l->getLight(c);
		}
		c.color *= v[0].color;

		// change in x per change in y
		float dx1_y = (float)(triScreen[2][X] - triScreen[0][X]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dx2_y = (float)(triScreen[1][X] - triScreen[0][X]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tricam[2][Z] - tricam[0][Z]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2_y = (tricam[1][Z] - tricam[0][Z]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
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
		int ymax = std::min(triScreen[1][Y], GraphicsBL::size[Y]);

		// start and end x values of scanline
		float x1 = (float)triScreen[0][X] + dx1_y * (float)(y - triScreen[0][Y]);
		float x2 = (float)triScreen[0][X] + dx2_y * (float)(y - triScreen[0][Y]);

		// start and end z values of scanline
		float z1 = tricam[0][Z] + dz1_y * (float)(y - triScreen[0][Y]);
		float z2 = tricam[0][Z] + dz2_y * (float)(y - triScreen[0][Y]);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				// change in _ per change in x
				float dz_x = (z2 - z1) / (x2 - x1);

				// clip x axis
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, GraphicsBL::size[X]);
				float z = z1 + dz_x * (x - (int)x1);

				int index = GraphicsBL::coordsToIndex({ x, y });
				for (x; x < xmax; x++) {
					if (z < GraphicsBL::depth[index]) {
						GraphicsBL::depth[index] = z;
						GraphicsBL::pixels_rgb[index] = c.color;
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
		ymax = std::min(triScreen[2][Y], GraphicsBL::size[Y]);

		*dx_y = (float)(triScreen[2][X]- triScreen[1][X]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		x1 = (float)triScreen[2][X] - dx1_y * (float)(triScreen[2][Y] - y);
		x2 = (float)triScreen[2][X] - dx2_y * (float)(triScreen[2][Y] - y);

		*dz_y = (tricam[2][Z]- tricam[1][Z]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1 = tricam[2][Z] - dz1_y * (float)(triScreen[2][Y] - y);
		z2 = tricam[2][Z] - dz2_y * (float)(triScreen[2][Y] - y);

		// draw second tri segment
		rasterSegment();
	}


	void Triangle::raster_v(Vertex v[3], Vec3f tricam[3]) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = GraphicsBL::camera.getScreenCoord(tricam[a]);
		}

		// sort tri pts by y ascending
		if (triScreen[0][Y] > triScreen[1][Y]) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tricam[0], tricam[1]);
			std::swap(v[0], v[1]);
		}
		if (triScreen[0][Y] > triScreen[2][Y]) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tricam[0], tricam[2]);
			std::swap(v[0], v[2]);
		}
		if (triScreen[1][Y] > triScreen[2][Y]) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tricam[1], tricam[2]);
			std::swap(v[1], v[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2][Y] < 0 || triScreen[0][Y] > GraphicsBL::size[Y]) return;

		// change in x per change in y
		float dx1_y = (float)(triScreen[2][X] - triScreen[0][X]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dx2_y = (float)(triScreen[1][X] - triScreen[0][X]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tricam[2][Z] - tricam[0][Z]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2_y = (tricam[1][Z] - tricam[0][Z]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// change in color per change in y
		Vec3f dc1_y = (v[2].color - v[0].color) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		Vec3f dc2_y = (v[1].color - v[0].color) / (float)(triScreen[1][Y] - triScreen[0][Y]);
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
		int ymax = std::min(triScreen[1][Y], GraphicsBL::size[Y]);

		// start and end x values of scanline
		float x1 = (float)triScreen[0][X] + dx1_y * (float)(y - triScreen[0][Y]);
		float x2 = (float)triScreen[0][X] + dx2_y * (float)(y - triScreen[0][Y]);

		// start and end z values of scanline
		float z1 = tricam[0][Z] + dz1_y * (float)(y - triScreen[0][Y]);
		float z2 = tricam[0][Z] + dz2_y * (float)(y - triScreen[0][Y]);

		// start and end color values of scanline
		Vec3f c1 = v[0].color + dc1_y * (float)(y - triScreen[0][Y]);
		Vec3f c2 = v[0].color + dc2_y * (float)(y - triScreen[0][Y]);

		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				// change in _ per change in x
				float dz_x = (z2 - z1) / (x2 - x1);
				Vec3f dc_x = (c2 - c1) / (x2 - x1);

				// clip x axis
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, GraphicsBL::size[X]);
				float z = z1 + dz_x * (x - (int)x1);
				Vec3f c = c1 + dc_x * (x - x1);

				int index = GraphicsBL::coordsToIndex({ x, y });
				for (x; x < xmax; x++) {
					if (z < GraphicsBL::depth[index]) {
						GraphicsBL::depth[index] = z;
						GraphicsBL::pixels_rgb[index] = c;
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
		ymax = std::min(triScreen[2][Y], GraphicsBL::size[Y]);

		*dx_y = (float)(triScreen[2][X] - triScreen[1][X]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		x1 = (float)triScreen[2][X] - dx1_y * (float)(triScreen[2][Y] - y);
		x2 = (float)triScreen[2][X] - dx2_y * (float)(triScreen[2][Y] - y);

		*dz_y = (tricam[2][Z] - tricam[1][Z]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1 = tricam[2][Z] - dz1_y * (float)(triScreen[2][Y] - y);
		z2 = tricam[2][Z] - dz2_y * (float)(triScreen[2][Y] - y);

		*dc_y = (v[2].color - v[1].color) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		c1 = v[2].color - dc1_y * (float)(triScreen[2][Y] - y);
		c2 = v[2].color - dc2_y * (float)(triScreen[2][Y] - y);

		// draw second tri segment
		rasterSegment();
	}


	void Triangle::raster_p(Vertex v[3], Vec3f tricam[3]) const {
		// project tri onto screen
		Vec2 triScreen[3] = { 0 };
		for (int a = 0; a < 3; a++) {
			triScreen[a] = GraphicsBL::camera.getScreenCoord(tricam[a]);
		}
		
		// sort tri pts by y ascending
		if (triScreen[0][Y] > triScreen[1][Y]) {
			std::swap(triScreen[0], triScreen[1]);
			std::swap(tricam[0], tricam[1]);
			std::swap(v[0], v[1]);
		}
		if (triScreen[0][Y] > triScreen[2][Y]) {
			std::swap(triScreen[0], triScreen[2]);
			std::swap(tricam[0], tricam[2]);
			std::swap(v[0], v[2]);
		}
		if (triScreen[1][Y] > triScreen[2][Y]) {
			std::swap(triScreen[1], triScreen[2]);
			std::swap(tricam[1], tricam[2]);
			std::swap(v[1], v[2]);
		}

		// early return if tri out of frame (y-axis)
		if (triScreen[2][Y] < 0 || triScreen[0][Y] > GraphicsBL::size[Y]) return;

		// divide tri_c by z to account for perspective
		float tri_zinv[3] = { 1.0f / tricam[0][Z], 1.0f / tricam[1][Z], 1.0f / tricam[2][Z] };
		for (int a = 0; a < 3; a++) {
			v[a].pos *= tri_zinv[a];
			v[a].normal *= tri_zinv[a];
		}

		// change in x per change in y
		float dx1_y = (float)(triScreen[2][X] - triScreen[0][X]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dx2_y = (float)(triScreen[1][X] - triScreen[0][X]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dx_y = &dx2_y;	// points to dx for side of triangle with pt1 (2 segments)

		// change in z per change in y
		float dz1_y = (tricam[2][Z] - tricam[0][Z]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2_y = (tricam[1][Z] - tricam[0][Z]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dz_y = &dz2_y;	// points to dz for side of triangle with pt1 (2 segments)

		// change in inverse z per change in y
		float dz1inv_y = (tri_zinv[2] - tri_zinv[0]) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		float dz2inv_y = (tri_zinv[1] - tri_zinv[0]) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		float* dzinv_y = &dz2inv_y;
		
		// change in pos per change in y
		Vec3f dp1_y = (v[2].pos - v[0].pos) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		Vec3f dp2_y = (v[1].pos - v[0].pos) / (float)(triScreen[1][Y] - triScreen[0][Y]);
		Vec3f* dp_y = &dp2_y;

		// change in normal per change in y
		Vec3f dn1_y = (v[2].normal - v[0].normal) / (float)(triScreen[2][Y] - triScreen[0][Y]);
		Vec3f dn2_y = (v[1].normal - v[0].normal) / (float)(triScreen[1][Y] - triScreen[0][Y]);
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
		int ymax = std::min(triScreen[1][Y], GraphicsBL::size[Y]);

		// start and end x values of scanline
		float x1 = (float)triScreen[0][X] + dx1_y * (float)(y - triScreen[0][Y]);
		float x2 = (float)triScreen[0][X] + dx2_y * (float)(y - triScreen[0][Y]);

		// start and end z values of scanline
		float z1 = tricam[0][Z] + dz1_y * (float)(y - triScreen[0][Y]);
		float z2 = tricam[0][Z] + dz2_y * (float)(y - triScreen[0][Y]);

		// start and end inverse z values of scanline
		float z1inv = tri_zinv[0] + dz1inv_y * (float)(y - triScreen[0][Y]);
		float z2inv = tri_zinv[0] + dz2inv_y * (float)(y - triScreen[0][Y]);

		// start and end pos values of scanline
		Vec3f p1 = v[0].pos + dp1_y * (float)(y - triScreen[0][Y]);
		Vec3f p2 = v[0].pos + dp2_y * (float)(y - triScreen[0][Y]);

		// start and end normal values of scanline
		Vec3f n1 = v[0].normal + dn1_y * (float)(y - triScreen[0][Y]);
		Vec3f n2 = v[0].normal + dn2_y * (float)(y - triScreen[0][Y]);
		
		// raster segment
		auto rasterSegment = [&]() {
			for (y; y < ymax; y++) {
				// change in _ per change in x
				float dz_x = (z2 - z1) / (x2 - x1);
				float dzinv_x = (z2inv - z1inv) / (x2 - x1);
				Vec3f dp_x = (p2 - p1) / (x2 - x1);
				Vec3f dn_x = (n2 - n1) / (x2 - x1);

				// clip x axis
				int x = std::max(0, (int)x1);
				int xmax = std::min((int)x2, GraphicsBL::size[X]);
				float z = z1 + dz_x * (x - (int)x1);
				float zinv = z1inv + dzinv_x * (x - (int)x1);
				Vec3f p = p1 + dp_x * (x - (int)x1);
				Vec3f n = n1 + dn_x * (x - (int)x1);

				int index = GraphicsBL::coordsToIndex({ x, y });
				for (x; x < xmax; x++) {
					if (z < GraphicsBL::depth[index]) {
						GraphicsBL::depth[index] = z;
						Vertex c = { p / zinv, getNormalized(n), GraphicsBL::light_ambient };
						for (auto& l : GraphicsBL::lights) {
							l->getLight(c);
						}
						c.color *= v[0].color;
						GraphicsBL::pixels_rgb[index] = c.color;
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
		ymax = std::min(triScreen[2][Y], GraphicsBL::size[Y]);

		*dx_y = (float)(triScreen[2][X] - triScreen[1][X]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		x1 = (float)triScreen[2][X] - dx1_y * (float)(triScreen[2][Y] - y);
		x2 = (float)triScreen[2][X] - dx2_y * (float)(triScreen[2][Y] - y);

		*dz_y = (tricam[2][Z] - tricam[1][Z]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1 = tricam[2][Z] - dz1_y * (float)(triScreen[2][Y] - y);
		z2 = tricam[2][Z] - dz2_y * (float)(triScreen[2][Y] - y);

		*dzinv_y = (tri_zinv[2] - tri_zinv[1]) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		z1inv = tri_zinv[2] - dz1inv_y * (float)(triScreen[2][Y] - y);
		z2inv = tri_zinv[2] - dz2inv_y * (float)(triScreen[2][Y] - y);

		*dp_y = (v[2].pos - v[1].pos) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		p1 = v[2].pos - dp1_y * (float)(triScreen[2][Y] - y);
		p2 = v[2].pos - dp2_y * (float)(triScreen[2][Y] - y);

		*dn_y = (v[2].normal - v[1].normal) / (float)(triScreen[2][Y] - triScreen[1][Y]);
		n1 = v[2].normal - dn1_y * (float)(triScreen[2][Y] - y);
		n2 = v[2].normal - dn2_y * (float)(triScreen[2][Y] - y);

		// draw second tri segment
		rasterSegment();
	}



















	/*

	auto rasterSegment = [&]() {
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			int xmax = std::min((int)x2, GraphicsBL::size[X]);

			float dz_x = (z2 - z1) / (x2 - x1);
			float z = z1 + dz_x * (x - (int)x1);

			int index = GraphicsBL::coordsToIndex({ x, y });

			for (x; x < xmax; x++) {
				if (z < GraphicsBL::depth[index]) {
					GraphicsBL::depth[index] = z;
					GraphicsBL::pixels_rgb[index] = v.color;
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


	auto rasterSegment = [&]() {
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			int xmax = std::min((int)x2, GraphicsBL::size[X]);

			float dz_x = (z2 - z1) / (x2 - x1);
			float z = z1 + dz_x * (x - (int)x1);

			Vec3f dc_x = (c2 - c1) / (x2 - x1);
			Vec3f c = c1 + dc_x * (x - x1);

			int index = GraphicsBL::coordsToIndex({ x, y });

			for (x; x < xmax; x++) {
				if (z < GraphicsBL::depth[index]) {
					GraphicsBL::depth[index] = z;
					GraphicsBL::pixels_rgb[index] = c;
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


	auto rasterSegment = [&]() {
		for (y; y < ymax; y++) {
			int x = std::max(0, (int)x1);
			int xmax = std::min((int)x2, GraphicsBL::size[X]);

			float dz_x = (z2 - z1) / (x2 - x1);
			float z = z1 + dz_x * (x - (int)x1);

			float dzinv_x = (z2inv - z1inv) / (x2 - x1);
			float zinv = z1inv + dzinv_x * (x - (int)x1);

			Vec3f dp_x = (p2 - p1) / (x2 - x1);
			Vec3f p = p1 + dp_x * (x - (int)x1);

			Vec3f dn_x = (n2 - n1) / (x2 - x1);
			Vec3f n = n1 + dn_x * (x - (int)x1);

			int index = GraphicsBL::coordsToIndex({ x, y });

			for (x; x < xmax; x++) {
				if (z < GraphicsBL::depth[index]) {
					GraphicsBL::depth[index] = z;
					Vertex v = { p / zinv, getNormalized(n), GraphicsBL::light_ambient };
					for (auto& l : GraphicsBL::lights) {
						l->getLight(v);
					}
					v.color *= pts[0]->color;
					GraphicsBL::pixels_rgb[index] = v.color;
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
	*/
















}
