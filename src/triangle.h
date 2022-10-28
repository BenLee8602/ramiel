#pragma once

#include <vector>
#include "graphics.h"


namespace ramiel {

	template<typename Vertex, class PixelShader>
	class Triangle {
		Vertex v[3];
		PixelShader& pixelShader;


		public:
		Triangle(
			PixelShader& pixelShader
			const Vertex& v1,
			const Vertex& v2,
			const Vertex& v3
		) {
			this->pixelShader = pixelShader;
			v[0] = v1;
			v[1] = v2;
			v[2] = v3;
		}


		private:
		void raster() {
			if (v[0].screenPos[Y] > v[1].screenPos[Y]) std::swap(v[0], v[1]);
			if (v[0].screenPos[Y] > v[2].screenPos[Y]) std::swap(v[0], v[2]);
			if (v[1].screenPos[Y] > v[2].screenPos[Y]) std::swap(v[1], v[2]);

			Vertex dy1 = (v[2] - v[0]) / (v[2].screenPos[Y] - v[0].screenPos[Y]);
			Vertex dy2 = (v[1] - v[0]) / (v[1].screenPos[Y] - v[0].screenPos[Y]);
			Vertex* dy;
			if (dy1.screenPos[X] > dy2.screenPos[X]) {
				std::swap(dy1, dy2);
				dy = &dy2;
			}
			else dy = &dy1;

			float y = std::max(0, v[0].screenPos[Y]);
			float ymax = std::min<float>(v[1].screenPos[Y], graphics::size[Y]);
			Vertex sc1 = v[0].screenPos[X] + dy1.screenPos[X] * (float)(y - v[0].screenPos[Y]);
			Vertex sc2 = v[0].screenPos[X] + dy2.screenPos[X] * (float)(y - v[0].screenPos[Y]);

			auto drawHalf = [&]() {
				for (y; y < ymax; ++y) {
					Vertex dx = (sc2 - sc1) / (sc2.screenPos[X] - sc1.screenPos[X]);

					float x = std::max(0, sc1.screenPos[X]);
					float xmax = std::min<float>(sc2.screenPos[X], graphics::size[X]);
					Vertex p = sc1 + dx * (x - sc1.screenPos[X]);
					size_t i = graphics::coordsToIndex(p.screenPos);

					for (x; x < xmax; ++x) {
						if (p.cameraPos[Z] < graphics::depth[i]) {
							graphics::depth[i] = p.cameraPos[Z];
							graphics::pixels[i] = pixelShader.draw(p);
						}
						++i;
						p += dx;
					}
					sc1 += dy1;
					sc2 += dy2;
				}
			};

			drawHalf();

			ymax = std::min<float>(v[2].screenPos[Y], graphics::size[Y]);
			*dy = (v[2] - v[1]) / (v[2].screenPos[Y] - v[1].screenPos[Y]);
			sc1 = v[2] - dy1 * (float)(v[2].screenPos[Y] - y);
			sc2 = v[2] - dy2 * (float)(v[2].screenPos[Y] - y);

			drawHalf();
		}


		private:
		void clip1(Vertex& v0, Vertex& v1, Vertex& v2) {
			// ratio of line clipped
			float c1 = (graphics::camera.znear - v1.cameraPos[Z]) / (v0.cameraPos[Z] - v1.cameraPos[Z]);
			float c2 = (graphics::camera.znear - v1.cameraPos[Z]) / (v2.cameraPos[Z] - v1.cameraPos[Z]);

			// new tri formed from quad
			Triangle newtri = Triangle(
				pixelShader,
				v0, v1, v2
			);

			// clip and raster new tri
			newtri.v[0] = v1 + (v0 - v1) * c1;
			newtri.v[1] = v1 + (v2 - v1) * c2;
			newtri.v[2] = v2;
			v1 = newtri.v[0];

			newtri.raster();
		}


		private:
		void clip2(Vertex& v0, Vertex& v1, Vertex& v2) {
			// ratio of line clipped
			float c1 = (graphics::camera.znear - v0.cameraPos[Z]) / (v1.cameraPos[Z] - v0.cameraPos[Z]);
			float c2 = (graphics::camera.znear - v2.cameraPos[Z]) / (v1.cameraPos[Z] - v2.cameraPos[Z]);

			// clip
			v0 = v0 + (v1 - v0) * c1;
			v2 = v2 + (v1 - v2) * c2;
		}


		private:
		bool clip() {
			using graphics::camera;
			if (v[0].cameraPos[Z] < camera.znear) {
				if (v[1].cameraPos[Z] < camera.znear) {
					if (v[2].cameraPos[Z] < camera.znear) return false;
					else clip2(v[1], v[2], v[0]);
				}
				else if (v[2].cameraPos[Z] < camera.znear) clip2(v[0], v[1], v[2]);
				else clip1(v[2], v[0], v[1]);
			}
			else if (v[1].cameraPos[Z] < camera.znear) {
				if (v[2].cameraPos[Z] < camera.znear) clip2(v[2], v[0], v[1]);
				else clip1(v[0], v[1], v[2]);
			}
			else if (v[2].cameraPos[Z] < camera.znear) clip1(v[1], v[2], v[0]);
			return true;
		}


		public:
		void draw() {
			// clip and draw
			pixelShader.init();
			if (clip()) raster();
		}

	};

}


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
