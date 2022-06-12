#pragma once

#include "physicsobj.h"
#include "model.h"
#include "texture.h"

namespace ramiel {

	enum class ShadingType_ : uint8_t {
		FLAT,
		VERTEX,
		PIXEL,
		PIXEL_S
	};

	class Entity {
		ShadingType_ shading;
		Vec3f color;
		Model* model;
		Texture* texture;
		Texture* normalMap;
		unsigned specularExponent;
		float specularIntensity;
		PhysicsObj* physicsObj;
		
	public:
		Entity(
			Model* model,
			Texture* texture = nullptr,
			Texture* normalMap = nullptr,
			ShadingType_ shading = ShadingType_::FLAT,
			Vec3f color = vec3f_255,
			unsigned specularExponent = 0U,
			float specularIntensity = 0.0f,
			PhysicsObj* physicsObj = nullptr
		);
		~Entity();
		void draw();
	};

}
