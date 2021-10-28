#include <utility>
#include "vec.h"

namespace bl {

	float getMagnitude(const Vec3f& v) {
		return sqrtf(v[x] * v[x] + v[y] * v[y] + v[z] * v[z]);
	}

	Vec3f getNormalized(const Vec3f& v) {
		return v / getMagnitude(v);
	}

	float dotProduct(const Vec3f& vec1, const Vec3f& vec2) {
		return vec1[x] * vec2[x] + vec1[y] * vec2[y] + vec1[z] * vec2[z];
	}

	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2) {
		Vec3f out;
		out[x] = vec1[y] * vec2[z] - vec1[z] * vec2[y];
		out[y] = vec1[z] * vec2[x] - vec1[x] * vec2[z];
		out[z] = vec1[x] * vec2[y] - vec1[y] * vec2[x];
		return out;
	}

	float dirSimilarity(const Vec3f& vec1, const Vec3f& vec2) {
		//Vec3f vec1n = vec1.getNormalized();
		//Vec3f vec2n = vec2.getNormalized();
		float sim = (dotProduct(vec1, vec2) + 1.0f) / 2.0f;
		return sim;
	}

	std::ostream& operator<<(std::ostream& os, const Vec3f& vec) {
		return os << vec[x] << ", " << vec[y] << ", " << vec[z];
	}


	void c_min(Vec3f& color, const Vec3f& min) {
		color[r] = std::min(color[r], min[r]);
		color[g] = std::min(color[g], min[g]);
		color[b] = std::min(color[b], min[b]);
	}

	void c_max(Vec3f& color, const Vec3f& max) {
		color[r] = std::max(color[r], max[r]);
		color[g] = std::max(color[g], max[g]);
		color[b] = std::max(color[b], max[b]);
	}

	void c_clamp(Vec3f& color) {
		color[r] = std::min(std::max(color[r], 0.0f), 255.0f);
		color[g] = std::min(std::max(color[g], 0.0f), 255.0f);
		color[b] = std::min(std::max(color[b], 0.0f), 255.0f);
	}

	int rgbToDec(const Vec3f& color) {
		return ((int)color[r] << 16) + ((int)color[g] << 8) + (int)color[b];
	}

}
