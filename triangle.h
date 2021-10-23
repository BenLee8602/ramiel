#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
	protected:
		Vertex* pts[3];
		bool clip_f(Vec3f* tri, const Vec3f& color_e) const;
		bool clip_v(Vec3f* tri, Vec3f* tri_c) const;
		void raster_f(Vec3f* tri, const Vec3f& color_e) const;
		void raster_v(Vec3f* tri, Vec3f* tri_c) const;
	public:
		Triangle(Vertex** pts);
		void draw_f(const Vec3f& color_e) const;
		void draw_v() const;
		Vec3f getNormal() const;
		Vertex* operator[](int index) const;
	};

}

#endif
