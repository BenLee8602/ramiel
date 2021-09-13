#ifndef BL_ENTITY_H
#define BL_ENTITY_H

#include "triangle.h"

namespace bl {

	class Entity {
	protected:
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		void calcVertexNormals();
	public:
		Entity(const char* filename, const Vec3f& pos);
		virtual void draw() const;
	};

}

#endif
