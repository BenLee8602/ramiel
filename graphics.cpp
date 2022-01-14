#include "graphics.h"

namespace bl {

	float  GraphicsBL::dtime = 0.0f;
	Camera GraphicsBL::camera;
	Bloom  GraphicsBL::bloom(50, true);

	Vec2   GraphicsBL::size = { 0 };
	Vec2   GraphicsBL::mid = { 0 };
	size_t GraphicsBL::bufferSize = 0;

	std::unique_ptr<int[]>   GraphicsBL::pixels;
	std::unique_ptr<Vec3f[]> GraphicsBL::pixels_rgb;
	std::unique_ptr<float[]> GraphicsBL::depth;

	std::vector<std::unique_ptr<Entity>> GraphicsBL::entities;
	std::vector<std::unique_ptr<Light>>  GraphicsBL::lights;
	std::vector<std::unique_ptr<Effect>> GraphicsBL::effects;
	Vec3f GraphicsBL::light_ambient = vec3f_0;
	Vec3f GraphicsBL::bg_color = vec3f_0;

	std::ofstream GraphicsBL::debug("output.txt");


	void GraphicsBL::setBufferSize(Vec2 newSize) {
		size = newSize;
		mid = size / 2;
		bufferSize = size[X] * size[Y];
		pixels.reset(new int[bufferSize]);
		pixels_rgb.reset(new Vec3f[bufferSize]);
		depth.reset(new float[bufferSize]);
		setFov(90);
	}


	void GraphicsBL::setFov(int fov) {
		camera.setFov(fov);
	}

	
	const void* GraphicsBL::renderFrame(float dtime) {
		std::fill(pixels_rgb.get(), pixels_rgb.get() + bufferSize, bg_color);
		std::fill(depth.get(), depth.get() + bufferSize, camera.zfar);

		GraphicsBL::dtime = dtime;
		camera.calcTrigValues();
		//lights[0]->move(cam.getpos()); // temp

		for (auto& e : entities) e->draw();
		for (auto& e : effects) e->applyEffect(pixels_rgb.get(), pixels_rgb.get());
		bloom.applyEffect(pixels_rgb.get(), pixels_rgb.get());

		for (size_t i = 0; i < bufferSize; i++) {
			notBloom(pixels_rgb[i]);
			pixels[i] = rgbToDec(pixels_rgb[i]);
		}

		return (const void*)pixels.get();
	}


	void GraphicsBL::addEntity(Entity* entity) {
		entities.push_back(std::unique_ptr<Entity>(entity));
	}


	void GraphicsBL::addLight(Light* light) {
		lights.push_back(std::unique_ptr<Light>(light));
	}


	void GraphicsBL::addEffect(Effect* effect) {
		effects.push_back(std::unique_ptr<Effect>(effect));
	}


	void GraphicsBL::setAmbientLightColor(Vec3f color) {
		c_max(color);
		light_ambient = color;
	}

	void GraphicsBL::setBackgroundColor(Vec3f color) {
		c_max(color);
		bg_color = color;
	}


	void GraphicsBL::removeEntity(unsigned int index) {
		if (entities.size()) {
			index %= entities.size();
			entities.erase(entities.begin() + index);
		}
	}


	void GraphicsBL::removeLight(unsigned int index) {
		if (lights.size()) {
			index %= lights.size();
			lights.erase(lights.begin() + index);
		}
	}


	int GraphicsBL::coordsToIndex(const Vec2& in) {
		return size[X] * in[Y] + in[X];
	}


	void GraphicsBL::drawLine(const Vec3f& start_w, const Vec3f& end_w, const Vec3f& color) {
		// get camera coords
		Vec3f start_c = camera.getCameraCoord(start_w);
		Vec3f end_c = camera.getCameraCoord(end_w);

		// clip z-axis
		if (start_c[Z] < camera.znear && end_c[Z] < camera.znear) return;
		if (start_c[Z] < camera.znear) {
			start_c[X] = start_c[X] + (end_c[X] - start_c[X]) * (camera.znear - start_c[Z]) / (end_c[Z] - start_c[Z]);
			start_c[Y] = start_c[Y] + (end_c[Y] - start_c[Y]) * (camera.znear - start_c[Z]) / (end_c[Z] - start_c[Z]);
			start_c[Z] = camera.znear;
		}
		else if (end_c[Z] < camera.znear) {
			end_c[X] = end_c[X] + (start_c[X] - end_c[X]) * (camera.znear - end_c[Z]) / (start_c[Z] - end_c[Z]);
			end_c[Y] = end_c[Y] + (start_c[Y] - end_c[Y]) * (camera.znear - end_c[Z]) / (start_c[Z] - end_c[Z]);
			end_c[Z] = camera.znear;
		}

		// get screen coords
		Vec2 start_s = camera.getScreenCoord(start_c);
		Vec2 end_s = camera.getScreenCoord(end_c);

		// draw
		drawLine(start_s, end_s, color);
	}

	void GraphicsBL::drawLine(Vec2 start, Vec2 end, const Vec3f& color) {
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
			p = pixels.get() + coordsToIndex(start);
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
			p = pixels.get() + coordsToIndex(start);
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
			p = pixels.get() + coordsToIndex(start);
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
			p = pixels.get() + coordsToIndex(start);
			for (int y = start[Y]; y < end[Y]; y++) {
				*p = c;
				p += size[X];
			}
		}
	}

}
