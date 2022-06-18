#pragma once

#include <vector>
#include "graphics.h"

namespace ramiel::triangle {
	
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
	static bool clip(Draw& draw) {
		
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
