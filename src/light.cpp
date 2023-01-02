#include <cmath>
#include "light.h"

namespace ramiel {

	Light::Light(Vec3f color, float intensity) {
		this->color = color;
		c_max(this->color);
		this->intensity = std::max(0.0f, intensity);
		this->color *= this->intensity;
	}

	DirectionalLight::DirectionalLight(
		Vec3f color,
		float intensity,
		Vec3f dir
	) : Light(color, intensity) {
		if (!dir) dir[Z] = 1.0f;
		this->dir = getNormalized(dir);
	}

	PointLight::PointLight(
		Vec3f color,
		float intensity,
		Vec3f pos,
		float falloff
	) : Light(color, intensity) {
		if (falloff < 0.0f) falloff = 1.0f;
		this->pos = pos;
		this->falloff = falloff;
	}

	SpotLight::SpotLight(
		Vec3f color,
		float intensity,
		Vec3f pos,
		Vec3f dir,
		float falloff,
		float width,
		float falloffExp
	) : PointLight(color, intensity, pos, falloff) {
		if (!dir[X] && !dir[Y] && !dir[Z]) dir[Z] = 1.0f;
		if (falloff < 0.0f) this->falloff = 0.1f;
		this->dir = getNormalized(dir);
		this->width = 1.0f - (width / 360.0f);
		this->falloffExp = falloffExp;
	}


	Vec3f DirectionalLight::getLight(
		const Vec3f& pos,
		const Vec3f& normal
	) const {
		return this->color * std::max(
			0.0f,
			-dotProduct(normal, this->dir)
		);
	}


	Vec3f PointLight::getLight(
		const Vec3f& pos,
		const Vec3f& normal
	) const {
		Vec3f vec = pos - this->pos;
		float d = getMagnitude(vec);
		vec *= 1.0f / d; // normalize
		float falloff = 1.0f / (this->falloff * d * d + 1.0f);
		return this->color * falloff * std::max(0.0f, -dotProduct(vec, normal));
	}


	Vec3f SpotLight::getLight(
		const Vec3f& pos,
		const Vec3f& normal
	) const {
		Vec3f vec = pos - this->pos;
		float d = getMagnitude(vec);
		vec *= 1.0f / d; // normalize

		float s = std::max(0.0f, dotProduct(vec, this->dir));
		if (s < width) return vec3f_0;

		float falloff = 1.0f / (this->falloff * d * d + 1.0f) * std::pow(s, this->falloffExp);
		return this->color * falloff * std::max(0.0f, -dotProduct(vec, normal));
	}


	Vec3f DirectionalLight::getLight(
		const Vec3f& pos,
		const Vec3f& normal,
		const Vec3f& cameraPos,
		unsigned specularExponent,
		float specularIntensity
	) const {
		// precalculate
		float dot = dotProduct(normal, this->dir);

		// diffuse
		float diffuse = std::max(0.0f, -dot);

		// specular
		Vec3f cameraVec = getNormalized(cameraPos - pos);
		Vec3f reflectedVec = this->dir - normal * dot * 2.0f;
		float specular = specularIntensity * std::pow(
			std::max(0.0f, dotProduct(reflectedVec, cameraVec)),
			specularExponent
		);

		return this->color * (diffuse + specular);
	}
	

	Vec3f PointLight::getLight(
		const Vec3f& pos,
		const Vec3f& normal,
		const Vec3f& cameraPos,
		unsigned specularExponent,
		float specularIntensity
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
		Vec3f cameraVec = getNormalized(cameraPos - pos);
		Vec3f reflectedVec = vec - normal * dot * 2.0f;
		float specular = specularIntensity * std::pow(
			std::max(0.0f, dotProduct(reflectedVec, cameraVec)),
			specularExponent
		);

		return this->color * falloff * (diffuse + specular);
	}


	Vec3f SpotLight::getLight(
		const Vec3f& pos,
		const Vec3f& normal,
		const Vec3f& cameraPos,
		unsigned specularExponent,
		float specularIntensity
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
		Vec3f cameraVec = getNormalized(cameraPos - pos);
		Vec3f reflectedVec = vec - normal * dot * 2.0f;
		float specular = specularIntensity * std::pow(
			std::max(0.0f, dotProduct(reflectedVec, cameraVec)),
			specularExponent
		);

		return this->color * falloff * (diffuse + specular);
	}


	Vec3f LightingList::getAllLight(const Vec3f& pos, const Vec3f& normal) const {
		Vec3f allLight = ambientLight;
		for (auto& l : lights)
			allLight += l->getLight(pos, normal);
		return allLight;
	}

	Vec3f LightingListSpecular::getAllLight(const Vec3f& pos, const Vec3f& normal) const {
		Vec3f allLight = ambientLight;
		for (auto& l : lights)
			allLight += l->getLight(pos, normal, cameraPos, specularExponent, specularIntensity);
		return allLight;
	}

}
