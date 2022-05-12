#pragma once

#include "../src/vec.h"
#include "../src/effects.h" // temp

namespace bl {

	enum class ShadingType : uint8_t {
		FLAT,
		VERTEX,
		PIXEL,
		PIXEL_S
	};

	enum class EffectType : uint8_t { // temp
		COLORSHIFT,
		FOG,
		GREYSCALE,
		BLUR
	};


    class GraphicsBL {
	public:
		static void setBufferSize(Vec2 size);
		static void setFov(unsigned fov);
		static void setControls(bool controls[12]);

		static void setAmbientLightColor(Vec3f color);
		static void setBackgroundColor(Vec3f color);

		static void renderFrame(float dtime = 0.0f);

		static void getFrameDEC(int* frame);
		static void getFrameRGB(uint8_t* frame);

		static bool loadTexture(const char* name, const char* filename);

		static bool addEntity(
			const char* filename,
			ShadingType shading = ShadingType::FLAT,
			Vec3f color = vec3f_255,
			// physics
			Vec3f pos          = vec3f_0,
			bool  collision    = false,
			float hbxrad       = 0.5f,
			float mass         = 1.0f,
			bool  movement     = false,
			Vec3f velocity     = vec3f_0,
			Vec3f acceleration = vec3f_0
		);
		static bool addEntity(
			const char* model,
			const char* texture,
			ShadingType shading = ShadingType::FLAT,
			// physics
			Vec3f pos          = vec3f_0,
			bool  collision    = false,
			float hbxrad       = 0.5f,
			float mass         = 1.0f,
			bool  movement     = false,
			Vec3f velocity     = vec3f_0,
			Vec3f acceleration = vec3f_0
		);

		static void addDirLight(
			Vec3f color = vec3f_255,
			Vec3f dir = { 0.0f, 0.0f, -1.0f }
		);
		static void addPointLight(
			Vec3f color = vec3f_255,
			Vec3f pos = vec3f_0,
			float falloff = 1.0f
		);
		static void addSpotLight(
			Vec3f color = vec3f_255,
			Vec3f pos = vec3f_0,
			Vec3f dir = { 0.0f, 0.0f, 1.0f },
			float falloff = 0.1f,
			float width = 30.0f,
			float falloffExp = 50.0f
		);

		static void addEffect(Effect* effect); // temp

		static void removeEntity(size_t index);
		static void removeLight(size_t index);
		static void removeEffect(size_t index);
	};

}
