#ifndef BL_ENTITY_H
#define BL_ENTITY_H

#include "triangle.h"

namespace bl {

	class Entity {
	protected:
		Vec3f color;
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		void calcVertexNormals();
		void calcVertexColor();
	public:
		Entity(const char* filename, Vec3f& pos, Vec3f& color);
		virtual void draw();
	};

}

#endif
