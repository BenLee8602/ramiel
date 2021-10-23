#include <algorithm>
#include "light.h"
#include "render.h"

namespace bl {

	Light::Light(Vec3f& color) {
		this->color.x = std::min(color.x, 255.0f);
		this->color.y = std::min(color.y, 255.0f);
		this->color.z = std::min(color.z, 255.0f);

		this->color.x = std::max(0.0f, this->color.x);
		this->color.y = std::max(0.0f, this->color.y);
		this->color.z = std::max(0.0f, this->color.z);
	}


	Light_Dir::Light_Dir(Vec3f& color, Vec3f& dir) : Light(color) {
		this->dir = dir.getNormalized();
	}

	void Light_Dir::getLight(Vertex& v) const {
		v.color += color * dirSimilarity(v.normal, dir);
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = pos.getNormalized();
	}


	Light_Pt::Light_Pt(Vec3f& color, Vec3f& pos, float falloff) : Light(color) {
		this->pos = pos;
		this->falloff = falloff;
	}

	void Light_Pt::getLight(Vertex& v) const {
		Vec3f vec = v.pos - pos;
		float d = vec.getMagnitude();
		float f = 1.0f / (falloff * d * d + 1.0f);
		v.color += color * f * (1.0f - dirSimilarity(vec.getNormalized(), v.normal));
	}

	void Light_Pt::move(const Vec3f& pos) {
		this->pos = pos;
	}


	int rgbToDec(Vec3f rgb) {
		return ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;
	}

}
