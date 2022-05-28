#include <cmath>
#include "ramiel_p.h"

namespace ramiel {

	Light::Light(Vec3f color) {
		this->color = color;
		c_max(this->color);
	}


	Light_Dir::Light_Dir(Vec3f color, Vec3f dir) : Light(color) {
		if (!dir) dir[Z] = -1.0f;
		this->dir = getNormalized(dir);
	}

	Vec3f Light_Dir::getLight(
		const Vec3f& pos, const Vec3f& normal,
		unsigned specularExponent, float specularIntensity
	) const {
		// precalculate
		float dot = dotProduct(normal, this->dir);

		// diffuse
		float diffuse = std::max(0.0f, -dot);

		// specular
		Vec3f cameraVec = getNormalized(graphics::camera.getpos() - pos);
		Vec3f reflectedVec = this->dir - normal * dot * 2.0f;
		float specular = specularIntensity * std::pow(
			std::max(0.0f, dotProduct(reflectedVec, cameraVec)),
			specularExponent
		);

		return this->color * (diffuse + specular);
	}

	void Light_Dir::move(const Vec3f& pos) {
		this->dir = getNormalized(pos);
	}


	Light_Pt::Light_Pt(Vec3f color, Vec3f pos, float falloff) : Light(color) {
		if (falloff < 0.0f) falloff = 1.0f;
		this->pos = pos;
		this->falloff = falloff;
	}

	Vec3f Light_Pt::getLight(
		const Vec3f& pos, const Vec3f& normal,
		unsigned specularExponent, float specularIntensity
	) const {
		// precalculate
		Vec3f vec = pos - this->pos;
		float d = getMagnitude(vec);
		vec *= 1.0f / d; // normalize

		float dot = dotProduct(vec, normal);

		float falloff = 1.0f / (this->falloff * d * d + 1.0f);

		// diffuse
		float diffuse = std::max(0.0f, -dot);

		// specular
		Vec3f cameraVec = getNormalized(graphics::camera.getpos() - pos);
		Vec3f reflectedVec = vec - normal * dot * 2.0f;
		float specular = specularIntensity * std::pow(
			std::max(0.0f, dotProduct(reflectedVec, cameraVec)),
			specularExponent
		);

		return this->color * falloff * (diffuse + specular);
	}

	void Light_Pt::move(const Vec3f& pos) {
		this->pos = pos;
	}


	Light_Sp::Light_Sp(Vec3f color, Vec3f pos, Vec3f dir, float falloff, float width, float falloffExp) : Light_Pt(color, pos, falloff) {
		if (!dir[X] && !dir[Y] && !dir[Z]) dir[Z] = 1.0f;
		if (falloff < 0.0f) this->falloff = 0.1f;
		this->dir = getNormalized(dir);
		this->width = 1.0f - (width / 360.0f);
		this->falloffExp = falloffExp;
	}

	Vec3f Light_Sp::getLight(
		const Vec3f& pos, const Vec3f& normal,
		unsigned specularExponent, float specularIntensity
	) const {
		// precalculate
		Vec3f vec = pos - this->pos;
		float d = getMagnitude(vec);
		vec *= 1.0f / d; // normalize

		float s = std::max(0.0f, dotProduct(vec, this->dir));
		if (s < width) return vec3f_0;

		float dot = dotProduct(vec, normal);

		float falloff = 1.0f / (this->falloff * d * d + 1.0f) * std::pow(s, this->falloffExp);

		// diffuse
		float diffuse = std::max(0.0f, -dot);

		// specular
		Vec3f cameraVec = getNormalized(graphics::camera.getpos() - pos);
		Vec3f reflectedVec = vec - normal * dot * 2.0f;
		float specular = specularIntensity * std::pow(
			std::max(0.0f, dotProduct(reflectedVec, cameraVec)),
			specularExponent
		);

		return this->color * falloff * (diffuse + specular);
	}

	void Light_Sp::rotate(const Vec3f& dir) {
		this->dir = getNormalized(dir);
	}

}
