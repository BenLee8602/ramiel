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
		void(ramiel::Entity::*draw)();

	private:
		void draw_flat();
		void draw_flat_textured();
		void draw_vertex();
		void draw_vertex_textured();
		void draw_pixel();
		void draw_pixel_textured();
		void draw_pixel_normalmapped();
		void draw_pixel_textured_normalmapped();
		void draw_pixel_smooth();
		void draw_pixel_smooth_textured();
	};

}
