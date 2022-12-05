#pragma once

#include "camera.h"

namespace ramiel {

	typedef void(*Effect)(Camera&);


	void ldr(Camera& camera);
	void hdr(Camera& camera);


	template<uint8_t R, uint8_t G, uint8_t B, unsigned start_, unsigned end_>
	void fog(Camera& camera) {
		constexpr Vec3f fogColor = { R, G, B };
		constexpr float fogStart = (float)start_;
		constexpr float fogEnd   = (float)end_;
		constexpr float fogFactor = 1.0f / (float)(end_ - start_);

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
