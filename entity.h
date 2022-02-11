#ifndef BL_ENTITY_H
#define BL_ENTITY_H

#include "triangle.h"
#include "physics.h"

namespace bl {

	enum class ShadingType : char {
		FLAT,
		VERTEX,
		PIXEL
	};

	class Entity {
		static size_t totalVerts;
		static size_t totalTris;

		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		
	public:
		ShadingType shading;
		Vec3f color;
		Physics physics;

	private:
		void calcVertexColor();

	public:
		static size_t getTotalVerts();
		static size_t getTotalTris();
		Entity(
			const char* filename,
			ShadingType shading = ShadingType::FLAT,
			Vec3f color = vec3f_255,
			Physics physics = Physics()
		);
		virtual void draw();
	};

}

#endif
