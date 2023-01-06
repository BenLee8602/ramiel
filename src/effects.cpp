#include <algorithm>
#include "effects.h"

namespace ramiel {

	constexpr Vec3f rgbToLum = {
		0.2126f / 255.0f,
		0.7152f / 255.0f,
		0.0722f / 255.0f
	};

	static inline float luminance(Vec3f rgb) {
		return dotProduct(rgb, rgbToLum);
	}

	inline void changeLuminance(Vec3f& rgb, float oldLum, float newLum) {
		rgb *= newLum / oldLum;
	}

	inline void changeLuminance(Vec3f& rgb, float lum) {
		changeLuminance(rgb, luminance(rgb), lum);
	}


	// extended reinhard applied to luminance
	void Hdr::run(Camera& camera) const {
		Camera::ColorBufferIterator color = camera.getColorBuffer();

		float maxLum = luminance(*std::max_element(
			color, color + camera.getBufferSize(),
			[](const Vec3f& p1, const Vec3f& p2) {
				return luminance(p1) < luminance(p2);
			}
		));

		if (maxLum <= 1.0f) return;

		float ml = 1.0f / (maxLum * maxLum);
		for (size_t i = 0; i < camera.getBufferSize(); ++i) {
			float oldLum = luminance(color[i]);
			float newLum = oldLum * (1.0f + oldLum * ml) / (1.0f + oldLum);
			changeLuminance(color[i], oldLum, newLum);
		}
	}


	void Fog::run(Camera& camera) const {
		Camera::ColorBufferIterator color = camera.getColorBuffer();
		Camera::DepthBufferIterator depth = camera.getDepthBuffer();

		for (size_t i = 0; i < camera.getBufferSize(); i++) {
			if (depth[i] <= fogStart) continue;
			if (depth[i] >= fogEnd) {
				color[i] = fogColor;
				continue;
			}
			float shift = (depth[i] - fogStart) * fogFactor;
			color[i] = color[i] * (1.0f - shift) + fogColor * shift;
		}
	}

}
