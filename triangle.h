#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
		Vec3f* pts[3];
		Vec3f getNormal() const;
		bool clip(Vec3f* tri) const;	// clips z-near plane
		void clip_1p(Vec3f* tri) const;
		void clip_2p(Vec3f* tri) const;
		void raster(Vec3f* tri) const;
	public:
		Triangle(Vec3f** pts);
		void draw() const;
	};

}

#endif
