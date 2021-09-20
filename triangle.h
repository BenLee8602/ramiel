#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
		Vertex* pts[3];
		bool clip(Vec3f* tri, Vec3f* tri_c, const Vec3f& color_e) const;	// clips z-near plane
		void clip_1p(Vec3f* tri, Vec3f* tri_c, const Vec3f& color_e) const;
		void clip_2p(Vec3f* tri, Vec3f* tri_c, const Vec3f& color_e) const;
		void raster(Vec3f* tri, Vec3f* tri_c, const Vec3f& color_e) const;
	public:
		Triangle(Vertex** pts);
		void draw(const Vec3f& color_e) const;
		Vec3f getNormal() const;
		Vertex* operator[](int index) const;
	};

}

#endif
