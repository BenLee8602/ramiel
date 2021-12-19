#include "render.h"

namespace bl {

	void Effect::enable(bool enabled) { this->enabled = enabled; }
	bool Effect::isEnabled() const { return enabled; }


	ColorShift::ColorShift(Vec3f color, float strength, bool enabled) : Effect(enabled) {
		c_clamp(color);
		this->color = color;
		this->strength = std::min(std::max(0.0f, strength), 1.0f);
	}

	const Vec3f& ColorShift::getColor() const { return color; }
	float ColorShift::getStrength() const { return strength; }

	void ColorShift::setColor(Vec3f color) {
		c_clamp(color);
		this->color = color;
	}
	void ColorShift::setStrength(float strength) {
		this->strength = std::min(std::max(0.0f, strength), 1.0f);
	}

	void ColorShift::applyEffect() const {
		float s2 = 1.0f - strength;
		Vec3f c = color * strength;
		for (size_t i = 0; i < RenderBL::bufferSize; i++) {
			RenderBL::pixels_rgb[i] *= s2;
			RenderBL::pixels_rgb[i] += c;
		}
	}


	Fog::Fog(float start, float end, Vec3f color, bool enabled) : Effect(enabled) {
		this->start = std::max(0.2f, start);
		this->end = std::max(start + 0.1f, end);
		this->fac = 1.0f / (end - start);
		c_clamp(color);
		this->color = color;
	}

	float Fog::getStart() const { return start; }
	float Fog::getEnd() const { return end; }
	const Vec3f& Fog::getColor() const { return color; }

	void Fog::setStart(float start) {
		this->start = std::max(0.2f, start);
		fac = 1.0f / (end - start);
	}
	void Fog::setEnd(float end) {
		this->end = std::max(start + 0.1f, end);
		fac = 1.0f / (end - start);
	}
	void Fog::setColor(Vec3f color) {
		c_clamp(color);
		this->color = color;
	}

	void Fog::applyEffect() const {
		if (!isEnabled()) return;
		for (size_t i = 0; i < RenderBL::bufferSize; i++) {
			if (RenderBL::depth[i] < start) continue;
			float shift = std::min(1.0f, (RenderBL::depth[i] - start) * fac);
			RenderBL::pixels_rgb[i] *= 1.0f - shift;
			RenderBL::pixels_rgb[i] += this->color * shift;
		}
	}


	void Greyscale::applyEffect() const {
		if (!isEnabled()) return;
		for (size_t i = 0; i < RenderBL::bufferSize; i++) {
			RenderBL::pixels_rgb[i] = ((
				RenderBL::pixels_rgb[i][R] +
				RenderBL::pixels_rgb[i][G] +
				RenderBL::pixels_rgb[i][B]) / 3.0f
			);
		}
	}


	void Blur::applyEffect() const {
		if (!isEnabled()) return;
		float r = 1.0f / (2 * rad + 1);
		Vec3f* buff = new Vec3f[RenderBL::bufferSize]();

		for (int _y = 0; _y < RenderBL::size[Y]; _y++) {
			int y_idx = _y * RenderBL::size[X];
			Vec3f acc = RenderBL::pixels_rgb[y_idx] * rad;
			for (int i = 0; i <= rad; i++) acc += RenderBL::pixels_rgb[y_idx + i];
			for (int _x = 0; _x < RenderBL::size[X]; _x++) {
				buff[y_idx + _x] = acc * r;
				acc -= RenderBL::pixels_rgb[y_idx + std::max(0, _x - rad)];
				acc += RenderBL::pixels_rgb[y_idx + std::min(RenderBL::size[X] - 1, _x + rad + 1)];
			}
		}

		for (int _x = 0; _x < RenderBL::size[X]; _x++) {
			Vec3f acc = buff[_x] * rad;
			for (int i = 0; i <= rad; i++) acc += buff[i * RenderBL::size[X] + _x];
			for (int _y = 0; _y < RenderBL::size[Y]; _y++) {
				int y_idx = _y * RenderBL::size[X];
				RenderBL::pixels_rgb[y_idx + _x] = acc * r;
				acc -= buff[std::max(0, _y - rad) * RenderBL::size[X] + _x];
				acc += buff[std::min(RenderBL::size[Y] - 1, _y + rad + 1) * RenderBL::size[X] + _x];
			}
		}

		delete[] buff;
	}

}
