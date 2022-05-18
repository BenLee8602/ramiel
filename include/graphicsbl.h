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


    namespace GraphicsBL {

		void setBufferSize(Vec2u size);
		void setFov(unsigned fov);
		void setControls(bool controls[12]);

		void setAmbientLightColor(Vec3f color);
		void setBackgroundColor(Vec3f color);

		void renderFrame(float dtime = 0.0f);

		void getFrameDEC(int* frame);
		void getFrameRGB(uint8_t* frame);

		bool loadTexture(const char* name, const char* filename);

		bool addEntity(
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
		bool addEntity(
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

		void addDirLight(
			Vec3f color = vec3f_255,
			Vec3f dir = { 0.0f, 0.0f, -1.0f }
		);
		void addPointLight(
			Vec3f color = vec3f_255,
			Vec3f pos = vec3f_0,
			float falloff = 1.0f
		);
		void addSpotLight(
			Vec3f color = vec3f_255,
			Vec3f pos = vec3f_0,
			Vec3f dir = { 0.0f, 0.0f, 1.0f },
			float falloff = 0.1f,
			float width = 30.0f,
			float falloffExp = 50.0f
		);

		void addEffect(Effect* effect); // temp

		void removeEntity(size_t index);
		void removeLight(size_t index);
		void removeEffect(size_t index);
		
	}

}
