#include "render.h"

namespace bl {

	std::vector<Entity> RenderBL::entities;
	int* RenderBL::pixels;
	Camera RenderBL::cam;
	Vec2 RenderBL::size;
	Vec2 RenderBL::mid;
	int RenderBL::bufferSize;
	float RenderBL::dtime;
	std::ofstream RenderBL::debug("output.txt");

	void RenderBL::setBuffer(void* pixelBuffer, int sizex, int sizey) {
		pixels = (int*)pixelBuffer;
		size.x = sizex;
		size.y = sizey;
		mid.x = sizex / 2;
		mid.y = sizey / 2;
		bufferSize = sizex * sizey * sizeof(int);
		setFov(90);
	}

	void RenderBL::setFov(int fov) {
		cam.setFov(fov);
	}

	void RenderBL::renderFrame(float dtime) {
		debug << dtime << std::endl;
		RenderBL::dtime = dtime;
		cam.getControls();
		cam.calcTrigValues();
		memset((void*)pixels, 0, bufferSize);
		for (auto& e : entities) {
			e.draw();
		}
	}

	void RenderBL::addEntity(const char* objfilename, float x, float y, float z) {
		Vec3f pos = { x, y, z };
		entities.push_back(Entity(objfilename, pos));
	}

	int RenderBL::coordsToIndex(const Vec2& in) {
		return size.x * in.y + in.x;
	}

}
