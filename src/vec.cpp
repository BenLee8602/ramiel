#include <algorithm>
#include "vec.h"

namespace ramiel {

	float getMagnitude(const Vec3f& v) {
		return sqrtf(v[X] * v[X] + v[Y] * v[Y] + v[Z] * v[Z]);
	}

	Vec3f getNormalized(const Vec3f& v) {
		return v / getMagnitude(v);
	}

	Vec3f getNormalized(const Vec3f& v, float magnitude) {
		return v / magnitude;
	}

	float dotProduct(const Vec3f& vec1, const Vec3f& vec2) {
		return vec1[X] * vec2[X] + vec1[Y] * vec2[Y] + vec1[Z] * vec2[Z];
	}

	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2) {
		Vec3f out;
		out[X] = vec1[Y] * vec2[Z] - vec1[Z] * vec2[Y];
		out[Y] = vec1[Z] * vec2[X] - vec1[X] * vec2[Z];
		out[Z] = vec1[X] * vec2[Y] - vec1[Y] * vec2[X];
		return out;
	}


	void c_min(Vec3f& color, const Vec3f& min) {
		color[R] = std::min(color[R], min[R]);
		color[G] = std::min(color[G], min[G]);
		color[B] = std::min(color[B], min[B]);
	}

	void c_max(Vec3f& color, const Vec3f& max) {
		color[R] = std::max(color[R], max[R]);
		color[G] = std::max(color[G], max[G]);
		color[B] = std::max(color[B], max[B]);
	}

	void c_clamp(Vec3f& color) {
		color[R] = std::min(std::max(color[R], 0.0f), 255.0f);
		color[G] = std::min(std::max(color[G], 0.0f), 255.0f);
		color[B] = std::min(std::max(color[B], 0.0f), 255.0f);
	}

}
