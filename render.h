#ifndef BL_RENDER_H
#define BL_RENDER_H

#include <fstream>
#include "entity.h"
#include "camera.h"

namespace bl {

	class RenderBL {
		static std::vector<Entity> entities;
		static int* pixels;
		static Camera cam;
		static Vec2 size;
		static Vec2 mid;
		static int bufferSize;
		static float dtime;
		static const float znear;
		static std::ofstream debug;

	public:
		static void setBuffer(void* pixelBuffer, int sizex, int sizey);
		static void setFov(int fov);
		static void renderFrame(float dtime = 0.0f);
		static void addEntity(const char* objfilename, float x = 0.0f, float y = 0.0f, float z = 0.0f);

		static int coordsToIndex(const Vec2& in);

		friend class Triangle;
		friend class Entity;
		friend class Camera;
	};

}

#endif