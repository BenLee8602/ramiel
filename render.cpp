#include "render.h"

namespace bl {

	std::vector<Entity> RenderBL::entities;
	std::vector<Light_Dir> RenderBL::lights_dir;
	std::vector<Light_Pt> RenderBL::lights_pt;
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
		memset((void*)pixels, 0, bufferSize);
		std::fill(depth, depth + (size.x * size.y), 1000.0f);
		RenderBL::dtime = dtime;
		cam.getControls();
		cam.calcTrigValues();
		lights_pt[0].moveSource(cam.getpos()); // temp
		for (auto& e : entities) {
			e.draw();
		}
	}

	void RenderBL::addEntity(const char* objfilename, float x, float y, float z, float r, float g, float b) {
		Vec3f pos = { x, y, z };
		Vec3f color = { r, g, b };
		if (!color) color = { 255.0f, 255.0f, 255.0f };
		entities.push_back(Entity(objfilename, pos, color));
	}

	void RenderBL::addLight_dir(float x, float y, float z, float r, float g, float b) {
		Vec3f dir = { x, y, z };
		Vec3f color = { r, g, b };
		if (!dir) dir = { 0.0f, 0.0f, -1.0f };
		if (!color) color = { 255.0f, 255.0f, 255.0f };
		lights_dir.push_back(Light_Dir(dir, color));
	}

	void RenderBL::addLight_pt(float x, float y, float z, float r, float g, float b, float falloff) {
		Vec3f source = { x, y, z };
		Vec3f color = { r, g, b };
		if (!color) color = { 255.0f, 255.0f, 255.0f };
		lights_pt.push_back(Light_Pt(source, color, falloff));
	}

	int RenderBL::coordsToIndex(const Vec2& in) {
		return size.x * in.y + in.x;
	}

}
