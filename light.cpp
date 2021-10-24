#include <algorithm>
#include "light.h"
#include "render.h"

namespace bl {

	Light::Light(Vec3f& color) {
		this->color = color;
		c_clamp(this->color);
	}


	Light_Dir::Light_Dir(Vec3f& color, Vec3f& dir) : Light(color) {
		this->dir = getNormalized(dir);
	}

	void Light_Dir::getLight(Vertex& v) const {
		v.color += color * dirSimilarity(v.normal, dir);
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = getNormalized(pos);
	}


	Light_Pt::Light_Pt(Vec3f& color, Vec3f& pos, float falloff) : Light(color) {
		this->pos = pos;
		this->falloff = falloff;
	}

	void Light_Pt::getLight(Vertex& v) const {
		Vec3f vec = v.pos - pos;
		float d = getMagnitude(vec);
		float f = 1.0f / (falloff * d * d + 1.0f);
		v.color += color * f * (1.0f - dirSimilarity(getNormalized(vec), v.normal));
	}

	void Light_Pt::move(const Vec3f& pos) {
		this->pos = pos;
	}

}
