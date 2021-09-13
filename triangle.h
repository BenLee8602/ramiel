#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
		Vertex* pts[3];
		bool clip(Vec3f* tri) const;	// clips z-near plane
		void clip_1p(Vec3f* tri) const;
		void clip_2p(Vec3f* tri) const;
		void raster(Vec3f* tri) const;
	public:
		Triangle(Vertex** pts);
		void draw() const;
		Vec3f getNormal() const;
		const Vertex* operator[](int index) const;
	};

}

#endif
