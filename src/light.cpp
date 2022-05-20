#include <cmath>
#include "light.h"

namespace bl {

	Light::Light(Vec3f color) {
		this->color = color;
		c_max(this->color);
	}


	Light_Dir::Light_Dir(Vec3f color, Vec3f dir) : Light(color) {
		if (!dir) dir[Z] = -1.0f;
		this->dir = getNormalized(dir);
	}

	Vec3f Light_Dir::getLight(const Vec3f& pos, const Vec3f& normal) const {
		return this->color * std::max(0.0f, dotProduct(normal, this->dir));
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = getNormalized(pos);
	}


	Light_Pt::Light_Pt(Vec3f color, Vec3f pos, float falloff) : Light(color) {
		if (falloff < 0.0f) falloff = 1.0f;
		this->pos = pos;
		this->falloff = falloff;
	}

	Vec3f Light_Pt::getLight(const Vec3f& pos, const Vec3f& normal) const {
		Vec3f vec = pos - this->pos;
		float d = getMagnitude(vec);
		float f = 1.0f / (falloff * d * d + 1.0f);
		return this->color * f * std::max(0.0f, -dotProduct(getNormalized(vec), normal));
	}

	void Light_Pt::move(const Vec3f& pos) {
		this->pos = pos;
	}


	Light_Sp::Light_Sp(Vec3f color, Vec3f pos, Vec3f dir, float falloff, float width, float falloffExp) : Light_Pt(color, pos, falloff) {
		if (!dir[X] && !dir[Y] && !dir[Z]) dir[Z] = 1.0f;
		if (falloff < 0.0f) this->falloff = 0.1f;
		this->dir = getNormalized(dir);
		this->width = 1.0f - (width / 360.0f);
		this->falloffExp = falloffExp;
	}

	Vec3f Light_Sp::getLight(const Vec3f& pos, const Vec3f& normal) const {
		Vec3f vec = pos - this->pos;
		float d = getMagnitude(vec);
		vec = getNormalized(vec);
		float s = std::max(0.0f, dotProduct(vec, this->dir));
		if (s < width) return vec3f_0;
		float f = 1.0f / (this->falloff * d * d + 1.0f) * std::pow(s, this->falloffExp);
		return this->color * f * std::max(0.0f, -dotProduct(vec, normal));
	}

	void Light_Sp::rotate(const Vec3f& dir) {
		this->dir = getNormalized(dir);
	}

}
