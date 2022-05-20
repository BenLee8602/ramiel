#pragma once

#include "physics.h"
#include "model.h"
#include "texture.h"

namespace bl {

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
		
	public:
		Physics physics;

	private:
		void calcVertexColor(
			std::vector<Vec3f>& v_color,
			const std::vector<Vec3f>& v_pos,
			const std::vector<Vec3f> n_normal
		);

	public:
		Entity(
			Model* model,
			Texture* texture = nullptr,
			ShadingType_ shading = ShadingType_::FLAT,
			Vec3f color = vec3f_255,
			Physics physics = Physics()
		);
		void draw();
	};

}
