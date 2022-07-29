#pragma once

#include "vec.h"

#define RAMIEL_EFFECT_ARGS           \
	Vec2u size, unsigned bufferSize, \
	PixelBufferIterator pixel,       \
	DepthBufferIterator depth

namespace ramiel {

	typedef std::vector<Vec3f>::iterator PixelBufferIterator;
	typedef std::vector<float>::iterator DepthBufferIterator;
	typedef void(*Effect)(RAMIEL_EFFECT_ARGS);


	void ldr(RAMIEL_EFFECT_ARGS);
	void hdr(RAMIEL_EFFECT_ARGS);


	template<uint8_t R, uint8_t G, uint8_t B, unsigned start_, unsigned end_>
	void fog(RAMIEL_EFFECT_ARGS) {
		constexpr Vec3f color = { R, G, B };
		constexpr float start = (float)start_;
		constexpr float end   = (float)end_;
		constexpr float fac = 1.0f / (float)(end_ - start_);

		for (size_t i = 0; i < bufferSize; i++) {
			if (depth[i] <= start) continue;
			if (depth[i] >= end) {
				pixel[i] = color;
				continue;
			}
			float shift = (depth[i] - start) * fac;
			pixel[i] = pixel[i] * (1.0f - shift) + color * shift;
		}
	}

}
