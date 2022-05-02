#ifndef BL_GRAPHICS_H
#define BL_GRAPHICS_H

#include <memory>
#include "entity.h"
#include "camera.h"
#include "light.h"
#include "effects.h"


namespace bl {

	class GraphicsBL_p {
	public:
		static float  dtime;
		static Camera camera;
		static Bloom  bloom;

		static Vec2   size;
		static Vec2   mid;
		static size_t bufferSize;

		static std::unique_ptr<Vec3f[]> pixels;
		static std::unique_ptr<float[]> depth;

		static std::vector<std::unique_ptr<Entity>> entities;
		static std::vector<std::unique_ptr<Light>>  lights;
		static std::vector<std::unique_ptr<Effect>> effects;
		static Vec3f light_ambient;
		static Vec3f bg_color;

		static void setBufferSize(Vec2 newSize);
		static void setFov(unsigned fov);

		static void renderFrame(float dtime = 0.0f);
		static void drawEntities();
		static void getCollisions();

		static void getFrameDEC(int* frame);
		static void getFrameRGB(uint8_t* frame);

		static void addEntity(Entity* entity);
		static void addLight(Light* light);
		static void addEffect(Effect* effect);

		static void setAmbientLightColor(Vec3f color = bg_color);
		static void setBackgroundColor(Vec3f color = light_ambient);

		static void removeEntity(size_t index);
		static void removeLight(size_t index);
		static void removeEffect(size_t index);

		static int coordsToIndex(const Vec2& in);
	};

}

#endif
