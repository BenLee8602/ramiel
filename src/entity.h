#ifndef BL_ENTITY_H
#define BL_ENTITY_H

#include "triangle.h"
#include "physics.h"

namespace bl {

	enum class ShadingType_ : uint8_t {
		FLAT,
		VERTEX,
		PIXEL,
		PIXEL_S
	};

	class Entity {
		static size_t totalVerts;
		static size_t totalTris;

		std::vector<Vec3f> v_pos;
		std::vector<Vec3f> v_normal;
		std::vector<Triangle> triangles;
		
	public:
		ShadingType_ shading;
		Vec3f color;
		Physics physics;

	private:
		void calcVertexColor(std::vector<Vec3f>& v_color);

	public:
		static size_t getTotalVerts();
		static size_t getTotalTris();
		Entity(
			const char* filename,
			ShadingType_ shading = ShadingType_::FLAT,
			Vec3f color = vec3f_255,
			Physics physics = Physics()
		);
		void draw();
	};

}

#endif
