#include "render.h"

namespace bl {

	std::vector<Entity> RenderBL::entities;
	std::vector<Light> RenderBL::lights;
	int* RenderBL::pixels;
	float* RenderBL::depth;
	Camera RenderBL::cam;
	Vec2 RenderBL::size;
	Vec2 RenderBL::mid;
	int RenderBL::bufferSize;
	float RenderBL::dtime;
	const float RenderBL::znear = 0.2f;
	std::ofstream RenderBL::debug("output.txt");

	void RenderBL::setBuffer(void* pixelBuffer, int sizex, int sizey) {
		pixels = (int*)pixelBuffer;
		size.x = sizex;
		size.y = sizey;
		mid.x = sizex / 2;
		mid.y = sizey / 2;
		bufferSize = sizex * sizey * sizeof(int);
		delete[] depth;
		depth = new float[sizex * sizey];
		setFov(90);
	}

	void RenderBL::setFov(int fov) {
		cam.setFov(fov);
	}

	void RenderBL::renderFrame(float dtime) {
		RenderBL::dtime = dtime;
		cam.getControls();
		cam.calcTrigValues();
		memset((void*)pixels, 0, bufferSize);
		std::fill(depth, depth + (size.x * size.y), 1000.0f);
		for (auto& e : entities) {
			e.draw();
		}
	}

	void RenderBL::addEntity(const char* objfilename, float x, float y, float z) {
		Vec3f pos = { x, y, z };
		entities.push_back(Entity(objfilename, pos));
	}

	void RenderBL::addLight(float x, float y, float z, float r, float g, float b) {
		Vec3f source = { x, y, z };
		Vec3f color = { r, g, b };
		if (!source) source = { 0.0f, 0.0f, -1.0f };
		if (!color) color = { 255.0f, 255.0f, 255.0f };
		lights.push_back(Light(source, color));
	}

	int RenderBL::coordsToIndex(const Vec2& in) {
		return size.x * in.y + in.x;
	}

}
