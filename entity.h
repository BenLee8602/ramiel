#ifndef BL_ENTITY_H
#define BL_ENTITY_H

#include "triangle.h"

namespace bl {

	enum class ShadingType : char {
		FLAT,
		VERTEX,
		PIXEL
	};

	class Entity {
		static size_t totalVerts;
		static size_t totalTris;
	protected:
		Vec3f color;
		ShadingType shading;
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		void calcVertexColor();
	public:
		Entity(const char* filename,
			ShadingType shading = ShadingType::FLAT,
			Vec3f pos = vec3f_0,
			Vec3f color = vec3f_255
		);
		virtual void draw();
	};

}

#endif
