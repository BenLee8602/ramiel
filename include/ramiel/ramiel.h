#pragma once

#include "../src/vec.h"
#include "../src/effects.h" // temp

namespace ramiel {

	enum class ShadingType : uint8_t {
		FLAT,
		VERTEX,
		PIXEL,
		PIXEL_S
	};

	enum class ColliderType : uint8_t {
		NONE,
		SPHERE,
		AABB
	};

	enum class LightType : uint8_t {
		DIR,
		POINT,
		SPOT
	};

	
	namespace graphics {

		void setBufferSize(Vec2u size);
		void setFov(unsigned fov);
		void setControls(bool controls[12]);

		void setAmbientLightColor(Vec3f color);
		void setBackgroundColor(Vec3f color);

		void renderFrame();

		void getFrameDEC(int* frame);
		void getFrameRGB(uint8_t* frame);

		bool loadMesh(
			std::string name,
			std::string filename,
			float scale = 1.0f,
			Vec3f pos = vec3f_0,
			Vec3f rot = vec3f_0
		);
		bool loadTexture(
			std::string name,
			std::string filename,
			char type = 'c'
		);

		bool addEntity(
			std::string mesh,
			ShadingType shading = ShadingType::FLAT,
			Vec3f color = vec3f_255,
			std::string texture = std::string(),
			std::string normalMap = std::string(),
			unsigned specularExponent = 8U,
			float specularIntensity = 0.2f,
			Vec3f pos    = vec3f_0,
			Vec3f rot    = vec3f_0,
			float scale  = 1.0f,
			bool dynamic = false,
			Vec3f posVel = vec3f_0,
			Vec3f rotVel = vec3f_0,
			Vec3f posAcc = vec3f_0,
			Vec3f rotAcc = vec3f_0,
			ColliderType colliderType = ColliderType::NONE,
			float mass = 1.0f,
			float hbxrad = 0.5f,
			Vec3f size = { 0.5f, 0.5f, 0.5f }
		);

		void addDirLight(
			Vec3f color = vec3f_255,
			Vec3f dir = { 0.0f, 0.0f, 1.0f }
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

	
	namespace physics {

		void simulatePhysics(float dtime = 0.0f);

	}

}
