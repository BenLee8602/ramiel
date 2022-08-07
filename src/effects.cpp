#include <algorithm>
#include "effects.h"

namespace ramiel {

    void ldr(RAMIEL_EFFECT_ARGS) {
		for (size_t i = 0; i < bufferSize; ++i) {
			// determine total extra
			Vec3f extra = pixel[i] - vec3f_255;
			c_max(extra);
			float totalExtra = extra[R] + extra[G] + extra[B];
			if (!totalExtra) continue;

			// determine empty space
			Vec3f room = vec3f_255 - pixel[i];
			c_max(room);
			float totalRoom = room[R] + room[G] + room[B];
			if (!totalRoom) continue;
			float trInv = 1.0f / totalRoom;

			// process input pt
			Vec3f roomRatio = room * trInv;
			pixel[i] += roomRatio * totalExtra;
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
	void hdr(RAMIEL_EFFECT_ARGS) {
		float maxLum = luminance(*std::max_element(
			pixel, pixel + bufferSize,
			[](const Vec3f& p1, const Vec3f& p2) {
				return luminance(p1) < luminance(p2);
			}
		));

		if (maxLum <= 1.0f) return;

		float ml = 1.0f / (maxLum * maxLum);
		for (size_t i = 0; i < bufferSize; ++i) {
			float oldLum = luminance(pixel[i]);
			float newLum = oldLum * (1.0f + oldLum * ml) / (1.0f + oldLum);
			changeLuminance(pixel[i], oldLum, newLum);
		}
	}

}
