#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
		size_t pts[3];
	public:
		Triangle(size_t pts[3]);
		size_t operator[](size_t index) const;


	private:
		template<class GraphicsBL, class Draw>
		void raster(Draw& draw) {
			draw.init();
			draw.calcd_y();
			if (draw.dx1_y > draw.dx2_y) draw.swapdy();
			draw.clipy();

			auto rasterSegment = [&draw]() {
				for (draw.y; draw.y < draw.ymax; draw.y++) {
					draw.calcd_x();
					draw.clipx();
					for (draw.x; draw.x < draw.xmax; draw.x++) {
						if (draw.z < GraphicsBL::depth[draw.index]) {
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


	private:
		template<class GraphicsBL, class Draw>
		bool clip(Draw& draw) {
			
			auto clip1 = [this, &draw]() {
				// ratio of line clipped
				float c1 = (GraphicsBL::camera.znear - draw.tricam[1][Z]) / (draw.tricam[0][Z] - draw.tricam[1][Z]);
				float c2 = (GraphicsBL::camera.znear - draw.tricam[1][Z]) / (draw.tricam[2][Z] - draw.tricam[1][Z]);

				// new tri formed from quad
				Draw draw2;
				std::copy(draw.v, draw.v + 3, draw2.v);
				std::copy(draw.tricam, draw.tricam + 3, draw2.tricam);

				// clip and raster new tri
				draw.clip1(c1, c2, draw2);
				raster<GraphicsBL>(draw2);
			};

			auto clip2 = [&draw]() {
				// ratio of line clipped
				float c1 = (GraphicsBL::camera.znear - draw.tricam[0][Z]) / (draw.tricam[1][Z] - draw.tricam[0][Z]);
				float c2 = (GraphicsBL::camera.znear - draw.tricam[2][Z]) / (draw.tricam[1][Z] - draw.tricam[2][Z]);

				// clip
				draw.clip2(c1, c2);
			};

			if (draw.tricam[0][Z] < GraphicsBL::camera.znear) {
				if (draw.tricam[1][Z] < GraphicsBL::camera.znear) {
					// case 2
					if (draw.tricam[2][Z] < GraphicsBL::camera.znear) {
						return false;
					}
					// case 6
					else {
						std::swap(draw.v[1], draw.v[2]);
						std::swap(draw.tricam[1], draw.tricam[2]);
						clip2();
					}
				}
				// case 8
				else if (draw.tricam[2][Z] < GraphicsBL::camera.znear) {
					clip2();
				}
				// case 3
				else {
					std::swap(draw.v[1], draw.v[0]);
					std::swap(draw.tricam[1], draw.tricam[0]);
					clip1();
				}
			}

			else if (draw.tricam[1][Z] < GraphicsBL::camera.znear) {
				// case 7
				if (draw.tricam[2][Z] < GraphicsBL::camera.znear) {
					std::swap(draw.v[1], draw.v[0]);
					std::swap(draw.tricam[1], draw.tricam[0]);
					clip2();
				}
				// case 4
				else {
					clip1();
				}
			}

			// case 5
			else if (draw.tricam[2][Z] < GraphicsBL::camera.znear) {
				std::swap(draw.v[1], draw.v[2]);
				std::swap(draw.tricam[1], draw.tricam[2]);
				clip1();
			}

			// case 1
			return true;
		}


	public:
		template<class GraphicsBL, class Draw>
		void draw(Draw& draw) {
			// backface culling
			Vec3f v1 = draw.tricam[1] - draw.tricam[0];
			Vec3f v2 = draw.tricam[2] - draw.tricam[0];
			Vec3f tricamNormal = crossProduct(v1, v2);
			if (dotProduct(draw.tricam[0], tricamNormal) >= 0.0f) return;

			// clip and raster
			if (clip<GraphicsBL>(draw)) raster<GraphicsBL>(draw);
		}
	};

}

#endif
