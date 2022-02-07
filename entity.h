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
		static size_t getTotalVerts();
		static size_t getTotalTris();
		Entity(
			const char* filename,
			ShadingType shading = ShadingType::FLAT,
			Vec3f pos = vec3f_0,
			Vec3f color = vec3f_255
		);
		virtual void draw();
	};

	class Entity_Static : public Entity {
	protected:
		Vec3f hbxpos;
		float hbxrad;
	public:
		Entity_Static(
			const char* filename,
			ShadingType shading = ShadingType::FLAT,
			Vec3f pos = vec3f_0,
			Vec3f color = vec3f_255,
			float hbxrad = 0.5f
		) : 
			hbxpos(pos),
			hbxrad(hbxrad),
			Entity(filename, shading, pos, color)
		{}
		virtual void draw() override;
	};

	class Entity_Dynamic : public Entity_Static {
	protected:
		float mass;
		Vec3f velocity;
		Vec3f acceleration;
	public:
		Entity_Dynamic(
			const char* filename,
			ShadingType shading = ShadingType::FLAT,
			Vec3f pos = vec3f_0,
			Vec3f color = vec3f_255,
			float hitrad = 0.5f,
			float mass = 1.0f,
			Vec3f velocity = vec3f_0,
			Vec3f acceleration = vec3f_0
		) : 
			mass(mass),
			velocity(velocity),
			acceleration(acceleration),
			Entity_Static(filename, shading, pos, color, hitrad)
		{}
		virtual void draw() override;
	};

}

#endif
