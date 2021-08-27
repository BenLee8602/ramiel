#include "triangle.h"
#include "render.h"

namespace bl {

	Triangle::Triangle(Vec3f& pt0, Vec3f& pt1, Vec3f& pt2) {
		pts[0] = pt0;
		pts[1] = pt1;
		pts[2] = pt2;
	}
	
	void Triangle::draw() const {
		Vec3f triCamera[3] = { 0.0f };
		Vec2 triScreen[3] = { 0 };

		for (int a = 0; a < 3; a++) {
			triCamera[a] = RenderBL::cam.getCameraCoord(pts[a]);
			triScreen[a] = RenderBL::cam.getScreenCoord(triCamera[a]);
			RenderBL::pixels[RenderBL::coordsToIndex(triScreen[a])] = 0xffffff;
		}
	}

}
