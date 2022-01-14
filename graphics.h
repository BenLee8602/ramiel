#ifndef BL_GRAPHICS_H
#define BL_GRAPHICS_H

#include <fstream>
#include <memory>
#include "entity.h"
#include "camera.h"
#include "light.h"
#include "effects.h"

namespace bl {

	class GraphicsBL {
	public:
		static float  dtime;
		static Camera camera;
		static Bloom  bloom;

	private:
		static Vec2   size;
		static Vec2   mid;
		static size_t bufferSize;

	private:
		static std::unique_ptr<int[]>   pixels;
		static std::unique_ptr<Vec3f[]> pixels_rgb;
		static std::unique_ptr<float[]> depth;

	private:
		static std::vector<std::unique_ptr<Entity>> entities;
		static std::vector<std::unique_ptr<Light>>  lights;
		static std::vector<std::unique_ptr<Effect>> effects;
		static Vec3f light_ambient;
		static Vec3f bg_color;

	private:
		static std::ofstream debug;

	public:
		static void setBufferSize(Vec2 newSize);
		static void setFov(int fov);
		static const void* renderFrame(float dtime = 0.0f);

		static void addEntity(Entity* entity);
		static void addLight(Light* light);
		static void addEffect(Effect* effect);

		static void setAmbientLightColor(Vec3f color = bg_color);
		static void setBackgroundColor(Vec3f color = light_ambient);

		static void removeEntity(unsigned int index);
		static void removeLight(unsigned int index);

		static int coordsToIndex(const Vec2& in);

		static void drawLine(const Vec3f& start_w, const Vec3f& end_w, const Vec3f& color = vec3f_255);
		static void drawLine(Vec2 start, Vec2 end, const Vec3f& color = vec3f_255);

		friend class Triangle;
		friend class Entity;
		friend class Camera;

		friend class ColorShift;
		friend class Fog;
		friend class Greyscale;
		friend class Blur;
		friend class Bloom;
	};

}

#endif
