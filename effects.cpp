#include "graphics.h"

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
		for (size_t i = 0; i < GraphicsBL::bufferSize; i++) {
			GraphicsBL::pixels_rgb[i] *= s2;
			GraphicsBL::pixels_rgb[i] += c;
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
		for (size_t i = 0; i < GraphicsBL::bufferSize; i++) {
			if (GraphicsBL::depth[i] < start) continue;
			float shift = std::min(1.0f, (GraphicsBL::depth[i] - start) * fac);
			GraphicsBL::pixels_rgb[i] *= 1.0f - shift;
			GraphicsBL::pixels_rgb[i] += this->color * shift;
		}
	}


	void Greyscale::applyEffect() const {
		if (!isEnabled()) return;
		for (size_t i = 0; i < GraphicsBL::bufferSize; i++) {
			GraphicsBL::pixels_rgb[i] = ((
				GraphicsBL::pixels_rgb[i][R] +
				GraphicsBL::pixels_rgb[i][G] +
				GraphicsBL::pixels_rgb[i][B]) / 3.0f
			);
		}
	}


	int Blur::getRad() const { return rad; }
	void Blur::setRad(int rad) { this->rad = rad; }

	void Blur::applyEffect() const {
		if (!isEnabled()) return;
		float r = 1.0f / (2 * rad + 1);
		Vec3f* buff = new Vec3f[GraphicsBL::bufferSize]();

		for (int y = 0; y < GraphicsBL::size[Y]; y++) {
			int y_idx = y * GraphicsBL::size[X];
			Vec3f acc = GraphicsBL::pixels_rgb[y_idx] * rad;
			for (int i = 0; i <= rad; i++) acc += GraphicsBL::pixels_rgb[y_idx + i];
			for (int x = 0; x < GraphicsBL::size[X]; x++) {
				buff[y_idx + x] = acc * r;
				acc -= GraphicsBL::pixels_rgb[y_idx + std::max(0, x - rad)];
				acc += GraphicsBL::pixels_rgb[y_idx + std::min(GraphicsBL::size[X] - 1, x + rad + 1)];
			}
		}

		for (int x = 0; x < GraphicsBL::size[X]; x++) {
			Vec3f acc = buff[x] * rad;
			for (int i = 0; i <= rad; i++) acc += buff[i * GraphicsBL::size[X] + x];
			for (int y = 0; y < GraphicsBL::size[Y]; y++) {
				int y_idx = y * GraphicsBL::size[X];
				GraphicsBL::pixels_rgb[y_idx + x] = acc * r;
				acc -= buff[std::max(0, y - rad) * GraphicsBL::size[X] + x];
				acc += buff[std::min(GraphicsBL::size[Y] - 1, y + rad + 1) * GraphicsBL::size[X] + x];
			}
		}

		delete[] buff;
	}

}
