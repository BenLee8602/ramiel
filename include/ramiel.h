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


	struct GraphicsArgs {
		const char* model;
		ShadingType shading;
		Vec3f color;
		const char* texture;
		const char* normalMap;
		unsigned specularExponent;
		float specularIntensity;

		GraphicsArgs(
			const char* model,
			ShadingType shading = ShadingType::FLAT,
			Vec3f color = vec3f_255,
			const char* texture = nullptr,
			const char* normalMap = nullptr,
			unsigned specularExponent = 8U,
			float specularIntensity = 0.2f
		) : 
			model(model),
			shading(shading),
			color(color),
			texture(texture),
			normalMap(normalMap),
			specularExponent(specularExponent),
			specularIntensity(specularIntensity)
		{}
	};


	struct DynamicsArgs {
		Vec3f pos;
		Vec3f rot;
		bool dynamic;
		Vec3f posVel;
		Vec3f rotVel;
		Vec3f posAcc;
		Vec3f rotAcc;

		DynamicsArgs(
			Vec3f pos    = vec3f_0,
			Vec3f rot    = vec3f_0,
			bool dynamic = false,
			Vec3f posVel = vec3f_0,
			Vec3f rotVel = vec3f_0,
			Vec3f posAcc = vec3f_0,
			Vec3f rotAcc = vec3f_0
		) : 
			pos(pos),
			rot(rot),
			dynamic(dynamic),
			posVel(posVel),
			rotVel(rotVel),
			posAcc(posAcc),
			rotAcc(rotAcc)
		{}
	};


	enum class ColliderType : uint8_t {
		NONE,
		SPHERE
	};


	struct CollisionArgs {
		ColliderType colliderType;
		float mass;
		float hbxrad;

		CollisionArgs(
			ColliderType colliderType = ColliderType::NONE,
			float mass = 1.0f,
			float hbxrad = 0.5f
		) : 
			colliderType(colliderType),
			mass(mass),
			hbxrad(hbxrad)
		{}
	};


    namespace graphics {

		void setBufferSize(Vec2u size);
		void setFov(unsigned fov);
		void setControls(bool controls[12]);

		void setAmbientLightColor(Vec3f color);
		void setBackgroundColor(Vec3f color);

		void renderFrame(float dtime = 0.0f);

		void getFrameDEC(int* frame);
		void getFrameRGB(uint8_t* frame);

		bool loadModel(
			const char* name,
			const char* filename,
			Vec3f pos = vec3f_0,
			Vec3f rot = vec3f_0
		);
		bool loadTexture(
			const char* name,
			const char* filename,
			char type = 'c'
		);

		bool addEntity(
			GraphicsArgs ga,
			DynamicsArgs da = DynamicsArgs(),
			CollisionArgs ca = CollisionArgs()
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

}
