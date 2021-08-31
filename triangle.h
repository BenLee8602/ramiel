#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
		Vec3f pts[3];
		Vec3f getNormal(const Vec3f* tri) const;
		bool clip(Vec3f* tri) const;	// clips z-near plane
		void raster(Vec3f* tri) const;
	public:
		Triangle();
		Triangle(Vec3f& pt0, Vec3f& pt1, Vec3f& pt2);
		void draw() const;
	};

}

#endif
