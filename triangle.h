#ifndef BL_TRIANGLE_H
#define BL_TRIANGLE_H

#include <vector>
#include "vec.h"

namespace bl {

	class Triangle {
		size_t pts[3];
		bool clip_f(Vertex v[3], Vec3f tricam[3]) const;
		bool clip_v(Vertex v[3], Vec3f tricam[3]) const;
		bool clip_p(Vertex v[3], Vec3f tricam[3]) const;
		void raster_f(Vertex v[3], Vec3f tricam[3]) const;
		void raster_v(Vertex v[3], Vec3f tricam[3]) const;
		void raster_p(Vertex v[3], Vec3f tricam[3]) const;
	public:
		Triangle(size_t pts[3]);
		void draw_f(Vertex v[3], Vec3f tricam[3]) const;
		void draw_v(Vertex v[3], Vec3f tricam[3]) const;
		void draw_p(Vertex v[3], Vec3f tricam[3]) const;
		size_t operator[](size_t index) const;
	};

}

#endif
