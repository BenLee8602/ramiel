#ifndef BL_ENTITY_H
#define BL_ENTITY_H

#include "triangle.h"

namespace bl { 

	class Entity {
	protected:
		Vec3f color;
		char shading;
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
		void calcVertexColor();
	public:
		Entity(const char* filename, Vec3f& pos, Vec3f& color, char shading);
		virtual void draw();
	};

}

#endif
