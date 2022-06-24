#pragma once

#include "physicsobj.h"
#include "mesh.h"
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
		Mesh* mesh;
		float scale;
		Texture* texture;
		Texture* normalMap;
		unsigned specularExponent;
		float specularIntensity;
		PhysicsObj* physicsObj;
		
	public:
		Entity(
			Mesh* mesh,
			float scale,
			Texture* texture,
			Texture* normalMap,
			ShadingType_ shading,
			Vec3f color,
			unsigned specularExponent,
			float specularIntensity,
			PhysicsObj* physicsObj
		);
		~Entity();
		void draw();
	};

}
