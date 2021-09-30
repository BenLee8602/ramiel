#include <algorithm>
#include "light.h"
#include "render.h"

namespace bl {

	Light::Light(Vec3f& color) {
		this->color.x = std::min(color.x, 255.0f);
		this->color.y = std::min(color.y, 255.0f);
		this->color.z = std::min(color.z, 255.0f);

		this->color.x = std::max(0.0f, this->color.x);
		this->color.y = std::max(0.0f, this->color.y);
		this->color.z = std::max(0.0f, this->color.z);
	}


	Light_Dir::Light_Dir(Vec3f& color, Vec3f& dir) : Light(color) {
		this->dir = dir.getNormalized();
	}

	void Light_Dir::getLight(Vertex& v) const {
		v.color += color * dirSimilarity(v.normal, dir);
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = pos.getNormalized();
	}


	Light_Pt::Light_Pt(Vec3f& color, Vec3f& pos, float falloff) : Light(color) {
		this->pos = pos;
		this->falloff = falloff;
	}

	void Light_Pt::getLight(Vertex& v) const {
		Vec3f vec = v.pos - pos;
		float d = vec.getMagnitude();
		float f = 1.0f / (falloff * d * d + 1.0f);
		v.color += color * f * (1.0f - dirSimilarity(vec.getNormalized(), v.normal));
	}

	void Light_Pt::move(const Vec3f& pos) {
		this->pos = pos;
	}


	int rgbToDec(Vec3f rgb) {
		return ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;
	}


	int RGB::getDec() {
		return (r << 16) + (g << 8) + b;
	}

	void RGB::setDec(int dec) {
		r = (dec >> 16) & 0xff;
		g = (dec >> 8) & 0xff;
		b = dec & 0xff;
	}

	void RGB::clamp() {
		r = std::min(r, 255ui16);
		g = std::min(g, 255ui16);
		b = std::min(b, 255ui16);
	}

	RGB& RGB::operator+=(uint16_t num) {
		r += num;
		g += num;
		b += num;
		return *this;
	}

	RGB& RGB::operator-=(uint16_t num) {
		r -= num;
		g -= num;
		b -= num;
		return *this;
	}

	RGB& RGB::operator*=(uint16_t num) {
		r *= num;
		g *= num;
		b *= num;
		return *this;
	}

	RGB& RGB::operator/=(uint16_t num) {
		r /= num;
		g /= num;
		b /= num;
		return *this;
	}

	RGB RGB::operator+(uint16_t num) const {
		RGB sum;
		sum.r = r + num;
		sum.g = g + num;
		sum.b = b + num;
		return sum;
	}

	RGB RGB::operator-(uint16_t num) const {
		RGB diff;
		diff.r = r - num;
		diff.g = g - num;
		diff.b = b - num;
		return diff;
	}

	RGB RGB::operator*(uint16_t num) const {
		RGB prod;
		prod.r = r * num;
		prod.g = g * num;
		prod.b = b * num;
		return prod;
	}

	RGB RGB::operator/(uint16_t num) const {
		RGB quot;
		quot.r = r / num;
		quot.g = g / num;
		quot.b = b / num;
		return quot;
	}

	RGB& RGB::operator+=(const RGB& other) {
		r += other.r;
		g += other.g;
		b += other.b;
		return *this;
	}

	RGB& RGB::operator-=(const RGB& other) {
		r -= other.r;
		g -= other.g;
		b -= other.b;
		return *this;
	}

	RGB& RGB::operator*=(const RGB& other) {
		r *= other.r;
		g *= other.g;
		b *= other.b;
		return *this;
	}

	RGB& RGB::operator/=(const RGB& other) {
		r /= other.r;
		g /= other.g;
		b /= other.b;
		return *this;
	}

	RGB RGB::operator+(const RGB& other) const {
		RGB sum;
		sum.r = r + other.r;
		sum.g = g + other.g;
		sum.b = b + other.b;
		return sum;
	}

	RGB RGB::operator-(const RGB& other) const {
		RGB diff;
		diff.r = r + other.r;
		diff.g = g + other.g;
		diff.b = b + other.b;
		return diff;
	}

	RGB RGB::operator*(const RGB& other) const {
		RGB prod;
		prod.r = r + other.r;
		prod.g = g + other.g;
		prod.b = b + other.b;
		return prod;
	}

	RGB RGB::operator/(const RGB& other) const {
		RGB quot;
		quot.r = r + other.r;
		quot.g = g + other.g;
		quot.b = b + other.b;
		return quot;
	}

}
