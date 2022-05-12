#pragma once

#include "triangle.h"
#include "physics.h"
#include "texture.h"

namespace bl {

	enum class ShadingType_ : uint8_t {
		FLAT,
		VERTEX,
		PIXEL,
		PIXEL_S
	};

	class Entity {
		std::vector<Vec3f> v_pos;
		std::vector<Vec3f> v_normal;
		std::vector<Vec2f> v_txt;

		std::vector<Vec3u> tri;
		std::vector<Vec3u> tri_txt;
		
	public:
		ShadingType_ shading;
		Vec3f color;
		Physics physics;
		Texture* texture;

	private:
		void calcVertexColor(std::vector<Vec3f>& v_color);

	public:
		static size_t getTotalVerts();
		static size_t getTotalTris();
		Entity(
			const char* filename,
			ShadingType_ shading = ShadingType_::FLAT,
			Vec3f color = vec3f_255,
			Physics physics = Physics(),
			Texture* texture = nullptr
		);
		void draw();
	};

}
