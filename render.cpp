#include "render.h"

namespace bl {

	std::vector<Entity> RenderBL::entities;
	Vec3f RenderBL::light_ambient;
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
		size.x = sizex;
		size.y = sizey;
		mid.x = sizex / 2;
		mid.y = sizey / 2;
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
		//lights[0]->move(cam.getpos()); // temp
		for (auto& e : entities) {
			e.draw();
		}
	}


	void RenderBL::addEntity(const char* objfilename, char shading, float x, float y, float z, float r, float g, float b) {
		Vec3f pos = { x, y, z };
		Vec3f color = { r, g, b };
		if (!color) color = { 255.0f, 255.0f, 255.0f };
		entities.push_back(Entity(objfilename, pos, color, shading));
	}


	void RenderBL::addLight(char type, float x, float y, float z, float r, float g, float b, float falloff) {
		Vec3f pos = { x, y, z };
		Vec3f color = { r, g, b };
		if (!color) color = { 255.0f, 255.0f, 255.0f };

		Light* lt;

		if (type == 'd') {
			if (!pos) pos = { 0.0f, 0.0f, -1.0f };
			lt = new Light_Dir(color, pos);
			lights.push_back(lt);
		}

		else if (type == 'p') {
			lt = new Light_Pt(color, pos, falloff);
			lights.push_back(lt);
		}
	}


	void RenderBL::setAmbientLightColor(float r, float g, float b) {
		r = std::min(std::max(0.0f, r), 255.0f);
		g = std::min(std::max(0.0f, g), 255.0f);
		b = std::min(std::max(0.0f, b), 255.0f);
		light_ambient = { r, g, b };
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
		return size.x * in.y + in.x;
	}

}
