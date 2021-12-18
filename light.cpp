#include <cmath>
#include "light.h"
#include "render.h"

namespace bl {

	Light::Light(Vec3f color) {
		this->color = color;
		c_clamp(this->color);
	}


	Light_Dir::Light_Dir(Vec3f color, Vec3f dir) : Light(color) {
		if (!dir[x] && !dir[y] && !dir[z]) dir[z] = -1.0f;
		this->dir = getNormalized(dir);
	}

	void Light_Dir::getLight(Vertex& v) const {
		v.color += color * dirSimilarity(v.normal, dir);
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = getNormalized(pos);
	}


	Light_Pt::Light_Pt(Vec3f color, Vec3f pos, float falloff) : Light(color) {
		if (falloff < 0.0f) falloff = 1.0f;
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


	Light_Sp::Light_Sp(Vec3f color, Vec3f pos, Vec3f dir, float falloff, float width, float falloffExp) : Light_Pt(color, pos, falloff) {
		if (!dir[x] && !dir[y] && !dir[z]) dir[z] = 1.0f;
		if (falloff < 0.0f) falloff = 0.1f;
		this->dir = getNormalized(dir);
		this->width = 1.0f - (width / 360.0f);
		this->falloffExp = falloffExp;
	}

	void Light_Sp::getLight(Vertex& v) const {
		Vec3f vec = v.pos - pos;
		float d = getMagnitude(vec);
		vec = getNormalized(vec);
		float s = dirSimilarity(vec, dir);
		if (s < width) return;
		float f = 1.0f / (falloff * d * d + 1.0f) * std::powf(s, falloffExp);
		v.color += color * f * (1.0f - dirSimilarity(vec, v.normal));
	}

	void Light_Sp::rotate(const Vec3f& dir) {
		this->dir = getNormalized(dir);
	}

}
