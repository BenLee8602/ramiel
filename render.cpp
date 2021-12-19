#include "render.h"

namespace bl {

	std::vector<std::unique_ptr<Entity>> RenderBL::entities;
	std::vector<std::unique_ptr<Light>> RenderBL::lights;
	Vec3f RenderBL::light_ambient = vec3f_0;
	int RenderBL::bg_color = 0;
	std::vector<std::unique_ptr<Effect>> RenderBL::effects;
	Vec3f* RenderBL::pixels_rgb = nullptr;
	int* RenderBL::pixels = nullptr;
	float* RenderBL::depth = nullptr;
	Camera RenderBL::cam;
	Vec2 RenderBL::size = { 0 };
	Vec2 RenderBL::mid = { 0 };
	int RenderBL::bufferSize = 0;
	float RenderBL::dtime = 0.0f;
	const float RenderBL::znear = 0.2f;
	const float RenderBL::zfar = 1000.0f;
	std::ofstream RenderBL::debug("output.txt");


	void RenderBL::setBuffer(void* pixelBuffer, int sizex, int sizey) {
		pixels = (int*)pixelBuffer;
		size[X] = sizex;
		size[Y] = sizey;
		mid[X] = sizex / 2;
		mid[Y] = sizey / 2;
		bufferSize = sizex * sizey;
		delete[] depth;
		depth = new float[bufferSize];
		delete[] pixels_rgb;
		pixels_rgb = new Vec3f[bufferSize];
		setFov(90);
	}


	void RenderBL::setFov(int fov) {
		cam.setFov(fov);
	}

	
	void RenderBL::renderFrame(float dtime) {
		std::fill(pixels, pixels + bufferSize, bg_color);
		std::fill(pixels_rgb, pixels_rgb + bufferSize, 0);
		std::fill(depth, depth + bufferSize, zfar);
		RenderBL::dtime = dtime;
		cam.getControls();
		cam.calcTrigValues();
		//lights[0]->move(cam.getpos()); // temp
		for (auto& e : entities) e->draw();
		for (auto& e : effects) e->applyEffect();
		for (int i = 0; i < bufferSize; i++) {
			pixels[i] = rgbToDec(pixels_rgb[i]);
		}
	}


	void RenderBL::addEntity(Entity* entity) {
		entities.push_back(std::unique_ptr<Entity>(entity));
	}


	void RenderBL::addLight(Light* light) {
		lights.push_back(std::unique_ptr<Light>(light));
	}


	void RenderBL::addEffect(Effect* effect) {
		effects.push_back(std::unique_ptr<Effect>(effect));
	}


	void RenderBL::setAmbientLightColor(Vec3f color) {
		c_clamp(color);
		light_ambient = color;
	}

	void RenderBL::setBackgroundColor(Vec3f color) {
		c_clamp(color);
		bg_color = rgbToDec(color);
	}


	void RenderBL::removeEntity(unsigned int index) {
		if (entities.size()) {
			index %= entities.size();
			entities.erase(entities.begin() + index);
		}
	}


	void RenderBL::removeLight(unsigned int index) {
		if (lights.size()) {
			index %= lights.size();
			lights.erase(lights.begin() + index);
		}
	}


	int RenderBL::coordsToIndex(const Vec2& in) {
		return size[X] * in[Y] + in[X];
	}


	void RenderBL::drawLine(const Vec3f& start_w, const Vec3f& end_w, const Vec3f& color) {
		// get camera coords
		Vec3f start_c = cam.getCameraCoord(start_w);
		Vec3f end_c = cam.getCameraCoord(end_w);

		// clip z-axis
		if (start_c[Z] < znear && end_c[Z] < znear) return;
		if (start_c[Z] < znear) {
			start_c[X] = start_c[X] + (end_c[X] - start_c[X]) * (znear - start_c[Z]) / (end_c[Z] - start_c[Z]);
			start_c[Y] = start_c[Y] + (end_c[Y] - start_c[Y]) * (znear - start_c[Z]) / (end_c[Z] - start_c[Z]);
			start_c[Z] = znear;
		}
		else if (end_c[Z] < znear) {
			end_c[X] = end_c[X] + (start_c[X] - end_c[X]) * (znear - end_c[Z]) / (start_c[Z] - end_c[Z]);
			end_c[Y] = end_c[Y] + (start_c[Y] - end_c[Y]) * (znear - end_c[Z]) / (start_c[Z] - end_c[Z]);
			end_c[Z] = znear;
		}

		// get screen coords
		Vec2 start_s = cam.getScreenCoord(start_c);
		Vec2 end_s = cam.getScreenCoord(end_c);

		// draw
		drawLine(start_s, end_s, color);
	}

	void RenderBL::drawLine(Vec2 start, Vec2 end, const Vec3f& color) {
		if (start[Y] > end[Y]) std::swap(start, end);
		else if (end[Y] < 0 || start[Y] > size[Y]) return;

		int* p;
		int c = rgbToDec(color);

		// horizontal line
		if (start[Y] == end[Y]) {
			if (start[X] > end[X]) std::swap(start, end);
			if (start[X] > size[X] || end[X] < 0) return;
			start[X] = std::max(start[X], 0);
			end[X] = std::min(end[X], size[X]);
			p = pixels + coordsToIndex(start);
			for (int x = start[X]; x < end[X]; x++) {
				*p++ = c;
			}
			return;
		}

		float dx_y = (float)(end[X] - start[X]) / (float)(end[Y] - start[Y]);

		// clip y-axis
		if (start[Y] < 0) {
			start[X] = start[X] - dx_y * start[Y];
			start[Y] = 0;
		}
		if (end[Y] > size[Y]) {
			end[X] = end[X] - dx_y * (end[Y] - size[Y]);
			end[Y] = size[Y];
		}

		float x = 0.0f;

		// +ve slope line
		if (dx_y > 0.0f) {
			// clip x-axis
			if (start[X] > size[X] || end[X] < 0) return;
			if (start[X] < 0) {
				start[Y] = start[Y] - start[X] / dx_y;
				start[X] = 0;
			}
			if (end[X] > size[X]) {
				end[Y] = end[Y] - (end[X] - size[X]) / dx_y;
				end[X] = size[X];
			}

			// draw
			p = pixels + coordsToIndex(start);
			for (int y = start[Y]; y < end[Y]; y++) {
				*p = c;
				for (x; x <= dx_y; x++) {
					*p++ = c;
				}
				x -= dx_y;
				p += size[X];
			}
		}
		
		// -ve slope line
		else if (dx_y < 0.0f) {
			// clip x-axis
			if (start[X] < 0 || end[X] > size[X]) return;
			if (end[X] < 0) {
				end[Y] = end[Y] - end[X] / dx_y;
				end[X] = 0;
			}
			if (start[X] > size[X]) {
				start[Y] = start[Y] - (start[X] - size[X]) / dx_y;
				start[X] = size[X];
			}

			// draw
			p = pixels + coordsToIndex(start);
			for (int y = start[Y]; y < end[Y]; y++) {
				*p = c;
				for (x; x >= dx_y; x--) {
					*p-- = c;
				}
				x -= dx_y;
				p += size[X];
			}
		}

		// vertical line
		else {
			if (start[X] > size[X] || end[X] < 0) return;
			p = pixels + coordsToIndex(start);
			for (int y = start[Y]; y < end[Y]; y++) {
				*p = c;
				p += size[X];
			}
		}
	}

}
