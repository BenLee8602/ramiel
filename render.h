#ifndef BL_RENDER_H
#define BL_RENDER_H

#include <fstream>
#include "entity.h"
#include "camera.h"
#include "light.h"

namespace bl {

	class RenderBL {
		static std::vector<Entity> entities;
		static std::vector<Light*> lights;
		static Vec3f light_ambient;
		static int bg_color;
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

		static void addEntity(const char* objfilename, char shading = 'f', Vec3f pos = vec3f_0, Vec3f color = vec3f_0);
		static void addLight(
			char type = 'd', Vec3f pos = vec3f_0, Vec3f color = vec3f_0,		// for directional lights
			float falloff = -1.0f,												// for point lights
			Vec3f dir = vec3f_0, float width = 30.0f, float falloffExp = 50.0f	// for spot lights
		);

		static void setAmbientLightColor(Vec3f color);
		static void setBackgroundColor(Vec3f color = light_ambient);

		static void removeEntity(unsigned int index);
		static void removeLight(unsigned int index);

		static int coordsToIndex(const Vec2& in);

		static void drawLine(const Vec3f& start_w, const Vec3f& end_w, const Vec3f& color = vec3f_255);
		static void drawLine(Vec2 start, Vec2 end, const Vec3f& color = vec3f_255);

		friend class Triangle;
		friend class Entity;
		friend class Camera;
	};

}

#endif
