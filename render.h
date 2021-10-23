#ifndef BL_RENDER_H
#define BL_RENDER_H

#include <fstream>
#include "entity.h"
#include "camera.h"
#include "light.h"

namespace bl {

	class RenderBL {
		static std::vector<Entity> entities;
		static Vec3f light_ambient;
		static std::vector<Light*> lights;
		static int* pixels;
		static float* depth;
		static Camera cam;
		static Vec2 size;
		static Vec2 mid;
		static int bufferSize;
		static float dtime;
		static const float znear;
		static const float zfar;
		static std::ofstream debug;

	public:
		static void setBuffer(void* pixelBuffer, int sizex, int sizey);
		static void setFov(int fov);
		static void renderFrame(float dtime = 0.0f);

		static void addEntity(
			const char* objfilename, char shading = 'f',
			float x = 0.0f, float y = 0.0f, float z = 0.0f,
			float r = 0.0f, float g = 0.0f, float b = 0.0f
		);
		static void addLight(
			char type = 'd', 
			float x = 0.0f, float y = 0.0f, float z = 0.0f,
			float r = 0.0f, float g = 0.0f, float b = 0.0f,
			float falloff = 1.0f
		);

		static void setAmbientLightColor(float r, float g, float b);

		static void removeEntity(unsigned int index);
		static void removeLight(unsigned int index);

		static int coordsToIndex(const Vec2& in);

		friend class Triangle;
		friend class Entity;
		friend class Camera;
	};

}

#endif
