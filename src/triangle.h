#pragma once

#include <vector>
#include "graphics.h"

namespace ramiel::triangle {

	template<class Vertex>
	void raster2(Vertex v[3]) {
		if (v[0].scrPos[Y] > v[1].scrPos[Y]) std::swap(v[0], v[1]);
		if (v[0].scrPos[Y] > v[2].scrPos[Y]) std::swap(v[0], v[2]);
		if (v[1].scrPos[Y] > v[2].scrPos[Y]) std::swap(v[1], v[2]);

		Vertex dy1 = (v[2] - v[0]) / (v[2].scrPos[Y] - v[0].scrPos[Y]);
		Vertex dy2 = (v[1] - v[0]) / (v[1].scrPos[Y] - v[0].scrPos[Y]);
		Vertex* dy;
		if (dy1.scrPos[X] > dy2.scrPos[X]) {
			std::swap(dy1, dy2);
			dy = &dy2;
		}
		else dy = &dy1;

		float y = std::max(0, v[0].scrPos[Y]);
		float ymax = std::min<float>(v[1].scrPos[Y], graphics::size[Y]);
		Vertex sc1 = v[0].scrPos[X] + dy1.scrPos[X] * (float)(y - v[0].scrPos[Y]);
		Vertex sc2 = v[0].scrPos[X] + dy2.scrPos[X] * (float)(y - v[0].scrPos[Y]);

		auto drawHalf = [&]() {
			for (y; y < ymax; ++y) {
				Vertex dx = (sc2 - sc1) / (sc2.scrPos[X] - sc1.scrPos[X]);

				float x = std::max(0, sc1.scrPos[X]);
				float xmax = std::min<float>(sc2.scrPos[X], graphics::size[X]);
				Vertex p = sc1 + dx * (x - sc1.scrPos[X]);

				for (x; x < xmax; ++x) {
					if (p.z < graphics::depth[graphics::coordsToIndex(p.scrPos)]) {
						graphics::pixels[graphics::coordsToIndex(p.scrPos)] = vec3f_255;
					}
					p += dx;
				}
				sc1 += dy1;
				sc2 += dy2;
			}
		};

		drawHalf();

		ymax = std::min<float>(v[2].scrPos[Y], graphics::size[Y]);
		*dy = (v[2] - v[1]) / (v[2].scrPos[Y] - v[1].scrPos[Y]);
		sc1 = v[2] - dy1 * (float)(v[2].scrPos[Y] - y);
		sc2 = v[2] - dy2 * (float)(v[2].scrPos[Y] - y);

		drawHalf();
	}


	template<class Vertex>
	void clipp1(Vertex& v0, Vertex& v1, Vertex& v2) {
		// ratio of line clipped
		float c1 = (graphics::camera.znear - v1.z) / (v0.z - v1.z);
		float c2 = (graphics::camera.znear - v1.z) / (v2.z - v1.z);

		// new tri formed from quad
		Vertex vnew[3] = { v0, v1, v2 };

		// clip and raster new tri
		vnew[0] = v1 + (v0 - v1) * c1;
		vnew[1] = v1 + (v2 - v1) * c2;
		vnew[2] = v2;
		v1 = vnew[0];

		raster(vnew);
	}


	template<class Vertex>
	void clipp2(Vertex& v0, Vertex& v1, Vertex& v2) {
		// ratio of line clipped
		float c1 = (graphics::camera.znear - v0.z) / (v1.z - v0.z);
		float c2 = (graphics::camera.znear - v2.z) / (v1.z - v2.z);

		// clip
		v0 = v0 + (v1 - v0) * c1;
		v2 = v2 + (v1 - v2) * c2;
	}


	template<class Vertex>
	bool clipp(Vertex v[3]) {
		using graphics::camera;
		if (v[0].z < camera.znear) {
			if (v[1].z < camera.znear) {
				if (v[2].z < camera.znear) return false;
				else clipp2(v[1], v[2], v[0]);
			}
			else if (v[2].z < camera.znear) clipp2(v[0], v[1], v[2]);
			else clipp1(v[2], v[0], v[1]);
		}
		else if (v[1].z < camera.znear) {
			if (v[2].z < camera.znear) clipp2(v[2], v[0], v[1]);
			else clipp1(v[0], v[1], v[2]);
		}
		else if (v[2].z < camera.znear) clipp1(v[1], v[2], v[0]);
		return true;
	}

	
	template<class Draw>
	void raster(Draw& draw) {
		draw.init();

		if (draw.triscreen[0][Y] > draw.triscreen[1][Y]) draw.swapv(0, 1);
		if (draw.triscreen[0][Y] > draw.triscreen[2][Y]) draw.swapv(0, 2);
		if (draw.triscreen[1][Y] > draw.triscreen[2][Y]) draw.swapv(1, 2);

		draw.calcd_y();
		if (draw.dx1_y > draw.dx2_y) draw.swapdy();
		draw.clipy();

		auto rasterSegment = [&draw]() {
			for (draw.y; draw.y < draw.ymax; draw.y++) {
				draw.calcd_x();
				draw.clipx();
				for (draw.x; draw.x < draw.xmax; draw.x++) {
					if (draw.z < graphics::depth[draw.index]) {
						draw.drawpixel();
					}
					draw.incx();
				}
				draw.incy();
			}
		};

		rasterSegment();
		draw.segmentswitch();
		rasterSegment();
	}


	template<class Draw>
	bool clip(Draw& draw) {
		
		auto clip1 = [&draw]() {
			// ratio of line clipped
			float c1 = (graphics::camera.znear - draw.tricam[1][Z]) / (draw.tricam[0][Z] - draw.tricam[1][Z]);
			float c2 = (graphics::camera.znear - draw.tricam[1][Z]) / (draw.tricam[2][Z] - draw.tricam[1][Z]);

			// new tri formed from quad
			Draw draw2 = draw;

			// clip and raster new tri
			draw.clip1(c1, c2, draw2);
			raster(draw2);
		};

		auto clip2 = [&draw]() {
			// ratio of line clipped
			float c1 = (graphics::camera.znear - draw.tricam[0][Z]) / (draw.tricam[1][Z] - draw.tricam[0][Z]);
			float c2 = (graphics::camera.znear - draw.tricam[2][Z]) / (draw.tricam[1][Z] - draw.tricam[2][Z]);

			// clip
			draw.clip2(c1, c2);
		};

		if (draw.tricam[0][Z] < graphics::camera.znear) {
			if (draw.tricam[1][Z] < graphics::camera.znear) {
				// case 2
				if (draw.tricam[2][Z] < graphics::camera.znear) {
					return false;
				}
				// case 6
				else {
					draw.swapv(1, 2);
					draw.swapv(0, 2);
					clip2();
				}
			}
			// case 8
			else if (draw.tricam[2][Z] < graphics::camera.znear) {
				clip2();
			}
			// case 3
			else {
				draw.swapv(1, 0);
				draw.swapv(2, 0);
				clip1();
			}
		}

		else if (draw.tricam[1][Z] < graphics::camera.znear) {
			// case 7
			if (draw.tricam[2][Z] < graphics::camera.znear) {
				draw.swapv(1, 0);
				draw.swapv(2, 0);
				clip2();
			}
			// case 4
			else {
				clip1();
			}
		}

		// case 5
		else if (draw.tricam[2][Z] < graphics::camera.znear) {
			draw.swapv(1, 2);
			draw.swapv(0, 2);
			clip1();
		}

		// case 1
		return true;
	}


	template<class Draw>
	void draw(Draw& draw) {
		// backface culling
		Vec3f v1 = draw.tricam[1] - draw.tricam[0];
		Vec3f v2 = draw.tricam[2] - draw.tricam[0];
		Vec3f tricamNormal = crossProduct(v1, v2);
		if (dotProduct(draw.tricam[0], tricamNormal) >= 0.0f) return;

		// clip and raster
		if (clip(draw)) raster(draw);
	}

}
