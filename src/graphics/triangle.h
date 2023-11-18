#pragma once

#include "camera.h"


namespace ramiel {

	template<typename Vertex, class PixelShader>
	class Triangle {
		Camera& camera;
		PixelShader pixelShader;
		Vertex v[3];

		public:
		Triangle(
			Camera& camera,
			PixelShader pixelShader
		) :
			camera(camera),
			pixelShader(pixelShader)
		{}


		private:
		void raster() {
			for (size_t i = 0; i < 3; ++i) v[i].screenPos = camera.getScreenCoord(v[i].cameraPos);

			Camera::ColorBufferIterator color = camera.getColorBuffer();
			Camera::DepthBufferIterator depth = camera.getDepthBuffer();
			
			if (v[0].screenPos[Y] > v[1].screenPos[Y]) std::swap(v[0], v[1]);
			if (v[0].screenPos[Y] > v[2].screenPos[Y]) std::swap(v[0], v[2]);
			if (v[1].screenPos[Y] > v[2].screenPos[Y]) std::swap(v[1], v[2]);

			Vertex dy1 = (v[2] - v[0]) / (v[2].screenPos[Y] - v[0].screenPos[Y]);
			Vertex dy2 = (v[1] - v[0]) / (v[1].screenPos[Y] - v[0].screenPos[Y]);
			Vertex* dy;
			if (dy1.screenPos[X] > dy2.screenPos[X]) {
				std::swap(dy1, dy2);
				dy = &dy1;
			}
			else dy = &dy2;

			int y = std::max<int>(0, v[0].screenPos[Y]);
			int ymax = std::min<int>(v[1].screenPos[Y], camera.getRes()[Y]);
			Vertex sc1 = v[0] + dy1 * (float)(y - v[0].screenPos[Y]);
			Vertex sc2 = v[0] + dy2 * (float)(y - v[0].screenPos[Y]);
			
			auto drawHalf = [&]() {
				for (y; y < ymax; ++y) {
					Vertex dx = (sc2 - sc1) / (sc2.screenPos[X] - sc1.screenPos[X]);

					int x = std::max<int>(0, sc1.screenPos[X]);
					int xmax = std::min<int>(sc2.screenPos[X], camera.getRes()[X]);
					Vertex p = sc1 + dx * (float)(x - sc1.screenPos[X]);
					size_t i = camera.getRes()[X] * y + x;

					for (x; x < xmax; ++x) {
						if (p.cameraPos[Z] < depth[i]) {
							depth[i] = p.cameraPos[Z];
							color[i] = pixelShader.draw(p);
						}
						++i;
						p += dx;
					}
					sc1 += dy1;
					sc2 += dy2;
				}
			};
			
			drawHalf();

			ymax = std::min<int>(v[2].screenPos[Y], camera.getRes()[Y]);
			*dy = (v[2] - v[1]) / (v[2].screenPos[Y] - v[1].screenPos[Y]);
			sc1 = v[2] - dy1 * (float)(v[2].screenPos[Y] - y);
			sc2 = v[2] - dy2 * (float)(v[2].screenPos[Y] - y);
			
			drawHalf();
		}


		private:
		void clip1(Vertex& v0, Vertex& v1, Vertex& v2) {
			// ratio of line clipped
			float c1 = (camera.znear - v1.cameraPos[Z]) / (v0.cameraPos[Z] - v1.cameraPos[Z]);
			float c2 = (camera.znear - v1.cameraPos[Z]) / (v2.cameraPos[Z] - v1.cameraPos[Z]);

			// new tri formed from quad
			Triangle newtri = Triangle(camera, pixelShader);

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
			float c1 = (camera.znear - v0.cameraPos[Z]) / (v1.cameraPos[Z] - v0.cameraPos[Z]);
			float c2 = (camera.znear - v2.cameraPos[Z]) / (v1.cameraPos[Z] - v2.cameraPos[Z]);

			// clip
			v0 = v0 + (v1 - v0) * c1;
			v2 = v2 + (v1 - v2) * c2;
		}


		private:
		bool clip() {
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
		void draw(
			const Vertex& v0,
			const Vertex& v1,
			const Vertex& v2
		) {
			v[0] = v0;
			v[1] = v1;
			v[2] = v2;
			pixelShader.init(v);
			if (clip()) raster();
		}

	};

}
