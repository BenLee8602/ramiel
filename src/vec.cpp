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

	int rgbToDec(const Vec3f& color) {
		return ((int)color[R] << 16) + ((int)color[G] << 8) + (int)color[B];
	}


	void ldr(std::vector<Vec3f>& pixels) {
		for (auto& p : pixels) {
			// determine total extra
			Vec3f extra = p - vec3f_255;
			c_max(extra);
			float totalExtra = extra[R] + extra[G] + extra[B];
			if (!totalExtra) continue;

			// determine empty space
			Vec3f room = vec3f_255 - p;
			c_max(room);
			float totalRoom = room[R] + room[G] + room[B];
			if (!totalRoom) {
				c_min(p);
				continue;
			}
			float trInv = 1.0f / totalRoom;

			// process input pt
			Vec3f roomRatio = room * trInv;
			p += roomRatio * totalExtra;
			c_min(p);
		}
	}


	constexpr Vec3f rgbToLum = {
		0.2126f / 255.0f,
		0.7152f / 255.0f,
		0.0722f / 255.0f
	};

	inline float luminance(Vec3f rgb) {
		return dotProduct(rgb, rgbToLum);
	}

	inline void changeLuminance(Vec3f& rgb, float oldLum, float newLum) {
		rgb *= newLum / oldLum;
	}

	inline void changeLuminance(Vec3f& rgb, float lum) {
		changeLuminance(rgb, luminance(rgb), lum);
	}

	// extended reinhard applied to luminance
	void hdr(std::vector<Vec3f>& pixels) {
		float maxLum = luminance(*std::max_element(
			pixels.begin(), pixels.end(),
			[](const Vec3f& p1, const Vec3f& p2) {
				return luminance(p1) < luminance(p2);
			}
		));

		if (maxLum <= 1.0f) {
			for (auto& p : pixels) c_min(p);
			return;
		}

		float ml = 1.0f / (maxLum * maxLum);
		for (auto& p : pixels) {
			float oldLum = luminance(p);
			float newLum = (oldLum * (1.0f + (oldLum / (maxLum * maxLum)))) / (1.0f + oldLum);
			changeLuminance(p, oldLum, newLum);
			c_min(p);
		}
	}

}
