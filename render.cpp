#include "render.h"

namespace bl {

	std::vector<Entity> RenderBL::entities;
	Vec3f RenderBL::light_ambient = vec3f_0;
	std::vector<Light*> RenderBL::lights;
	int* RenderBL::pixels;
	float* RenderBL::depth;
	Camera RenderBL::cam;
	Vec2 RenderBL::size;
	Vec2 RenderBL::mid;
	int RenderBL::bufferSize;
	float RenderBL::dtime;
	const float RenderBL::znear = 0.2f;
	const float RenderBL::zfar = 1000.0f;
	std::ofstream RenderBL::debug("output.txt");


	void RenderBL::setBuffer(void* pixelBuffer, int sizex, int sizey) {
		pixels = (int*)pixelBuffer;
		size[x] = sizex;
		size[y] = sizey;
		mid[x] = sizex / 2;
		mid[y] = sizey / 2;
		bufferSize = sizex * sizey;
		delete[] depth;
		depth = new float[sizex * sizey];
		setFov(90);
	}


	void RenderBL::setFov(int fov) {
		cam.setFov(fov);
	}

	
	void RenderBL::renderFrame(float dtime) {
		std::fill(pixels, pixels + bufferSize, 0);
		std::fill(depth, depth + bufferSize, zfar);
		RenderBL::dtime = dtime;
		cam.getControls();
		cam.calcTrigValues();
		lights[0]->move(cam.getpos()); // temp
		for (auto& e : entities) {
			e.draw();
		}
	}


	void RenderBL::addEntity(const char* objfilename, char shading, float _x, float _y, float _z, float _r, float _g, float _b) {
		Vec3f pos = { _x, _y, _z };
		Vec3f color = { _r, _g, _b };
		if (!_r && !_g && !_b) color = vec3f_255;
		entities.push_back(Entity(objfilename, pos, color, shading));
	}


	void RenderBL::addLight(char type, float _x, float _y, float _z, float _r, float _g, float _b, float falloff) {
		Vec3f pos = { _x, _y, _z };
		Vec3f color = { _r, _g, _b };
		if (!_r && !_g && !_b) color = vec3f_255;

		Light* lt;

		if (type == 'd') {
			if (!pos[x] && !pos[y] && !pos[z]) pos = { 0.0f, 0.0f, -1.0f };
			lt = new Light_Dir(color, pos);
			lights.push_back(lt);
		}

		else if (type == 'p') {
			lt = new Light_Pt(color, pos, falloff);
			lights.push_back(lt);
		}
	}


	void RenderBL::setAmbientLightColor(float _r, float _g, float _b) {
		light_ambient = { _r, _g, _b };
		c_clamp(light_ambient);
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
			delete lights[index];
			lights.erase(lights.begin() + index);
		}
	}


	int RenderBL::coordsToIndex(const Vec2& in) {
		return size[x] * in[y] + in[x];
	}


	void RenderBL::drawLine(const Vec3f& start_w, const Vec3f& end_w, const Vec3f& color) {
		// get camera coords
		Vec3f start_c = cam.getCameraCoord(start_w);
		Vec3f end_c = cam.getCameraCoord(end_w);

		// clip z-axis
		if (start_c[z] < znear && end_c[z] < znear) return;
		if (start_c[z] < znear) {
			start_c[x] = start_c[x] + (end_c[x] - start_c[x]) * (znear - start_c[z]) / (end_c[z] - start_c[z]);
			start_c[y] = start_c[y] + (end_c[y] - start_c[y]) * (znear - start_c[z]) / (end_c[z] - start_c[z]);
			start_c[z] = znear;
		}
		else if (end_c[z] < znear) {
			end_c[x] = end_c[x] + (start_c[x] - end_c[x]) * (znear - end_c[z]) / (start_c[z] - end_c[z]);
			end_c[y] = end_c[y] + (start_c[y] - end_c[y]) * (znear - end_c[z]) / (start_c[z] - end_c[z]);
			end_c[z] = znear;
		}

		// get screen coords
		Vec2 start_s = cam.getScreenCoord(start_c);
		Vec2 end_s = cam.getScreenCoord(end_c);

		// draw
		drawLine(start_s, end_s, color);
	}

	void RenderBL::drawLine(Vec2 start, Vec2 end, const Vec3f& color) {
		if (start[y] > end[y]) std::swap(start, end);
		else if (end[y] < 0 || start[y] > size[y]) return;

		int* p;
		int c = rgbToDec(color);

		// horizontal line
		if (start[y] == end[y]) {
			if (start[x] > end[x]) std::swap(start, end);
			if (start[x] > size[x] || end[x] < 0) return;
			start[x] = std::max(start[x], 0);
			end[x] = std::min(end[x], size[x]);
			p = pixels + coordsToIndex(start);
			for (int _x = start[x]; _x < end[x]; _x++) {
				*p++ = c;
			}
			return;
		}

		float dx_y = (float)(end[x] - start[x]) / (float)(end[y] - start[y]);

		// clip y-axis
		if (start[y] < 0) {
			start[x] = start[x] - dx_y * start[y];
			start[y] = 0;
		}
		if (end[y] > size[y]) {
			end[x] = end[x] - dx_y * (end[y] - size[y]);
			end[y] = size[y];
		}

		float _x = 0.0f;

		// +ve slope line
		if (dx_y > 0.0f) {
			// clip x-axis
			if (start[x] > size[x] || end[x] < 0) return;
			if (start[x] < 0) {
				start[y] = start[y] - start[x] / dx_y;
				start[x] = 0;
			}
			if (end[x] > size[x]) {
				end[y] = end[y] - (end[x] - size[x]) / dx_y;
				end[x] = size[x];
			}

			// draw
			p = pixels + coordsToIndex(start);
			for (int _y = start[y]; _y < end[y]; _y++) {
				*p = c;
				for (_x; _x <= dx_y; _x++) {
					*p++ = c;
				}
				_x -= dx_y;
				p += size[x];
			}
		}
		
		// -ve slope line
		else if (dx_y < 0.0f) {
			// clip x-axis
			if (start[x] < 0 || end[x] > size[x]) return;
			if (end[x] < 0) {
				end[y] = end[y] - end[x] / dx_y;
				end[x] = 0;
			}
			if (start[x] > size[x]) {
				start[y] = start[y] - (start[x] - size[x]) / dx_y;
				start[x] = size[x];
			}

			// draw
			p = pixels + coordsToIndex(start);
			for (int _y = start[y]; _y < end[y]; _y++) {
				*p = c;
				for (_x; _x >= dx_y; _x--) {
					*p-- = c;
				}
				_x -= dx_y;
				p += size[x];
			}
		}

		// vertical line
		else {
			if (start[x] > size[x] || end[x] < 0) return;
			p = pixels + coordsToIndex(start);
			for (int _y = start[y]; _y < end[y]; _y++) {
				*p = c;
				p += size[x];
			}
		}
	}

}
