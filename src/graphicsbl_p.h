#pragma once

#include <unordered_map>
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

		static std::vector<Vec3f> pixels;
		static std::vector<float> depth;


		static std::unordered_map<std::string, Texture*> textures;

		static std::vector<Entity*> entities;
		static std::vector<Light*>  lights;
		static std::vector<Effect*> effects;
		static Vec3f light_ambient;
		static Vec3f bg_color;

		static void setBufferSize(Vec2 newSize);
		static void setFov(unsigned fov);

		static void setAmbientLightColor(Vec3f color = bg_color);
		static void setBackgroundColor(Vec3f color = light_ambient);

		static void renderFrame(float dtime = 0.0f);
		static void drawEntities();
		static void getCollisions();

		static void getFrameDEC(int* frame);
		static void getFrameRGB(uint8_t* frame);

		static void loadTexture(const char* name, const char* filename);

		static void addEntity(Entity* entity);
		static void addLight(Light* light);
		static void addEffect(Effect* effect);

		static void removeEntity(size_t index);
		static void removeLight(size_t index);
		static void removeEffect(size_t index);

		static int coordsToIndex(const Vec2& in);
	};

}
