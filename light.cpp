#include <algorithm>
#include "light.h"
#include "render.h"

namespace bl {

	void Light_Dir::getLight(Vertex& v) const {
		v.color += color * dirSimilarity(v.normal, dir);
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = pos;
	}


	void Light_Pt::getLight(Vertex& v) const {
		Vec3f vec = v.pos - pos;
		float d = vec.getMagnitude();
		float f = 1.0f / (falloff * d * d + 1.0f);
		v.color += color * f * (1.0f - dirSimilarity(vec, v.normal));
	}

	void Light_Pt::move(const Vec3f& pos) {
		this->pos = pos;
	}


	int rgbToDec(Vec3f& rgb) {
		return ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;
	}

}
