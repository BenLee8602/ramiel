#pragma once

#include "camera.h"

namespace ramiel {

	class Light {
	protected:
		Vec3f color;
		float intensity;
	public:
		Light(Vec3f color, float intensity);
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal
		) const = 0;
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal,
			const Camera& camera,
			unsigned specularExponent,
			float specularIntensity
		) const = 0;
	};

	class Light_Dir : public Light {
	protected:
		Vec3f dir;
	public:
		Light_Dir(Vec3f color, float intensity, Vec3f dir);
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal
		) const override;
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal,
			const Camera& camera,
			unsigned specularExponent,
			float specularIntensity
		) const override;
	};

	class Light_Pt : public Light {
	protected:
		Vec3f pos;
		float falloff;
	public:
		Light_Pt(Vec3f color, float intensity, Vec3f pos, float falloff);
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal
		) const override;
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal,
			const Camera& camera,
			unsigned specularExponent,
			float specularIntensity
		) const override;
	};

	class Light_Sp : public Light_Pt {
		Vec3f dir;
		float width;
		float falloffExp;
	public:
		Light_Sp(
			Vec3f color,
			float intensity,
			Vec3f pos,
			Vec3f dir,
			float falloff,
			float width,
			float falloffExp
		);
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal
		) const override;
		virtual Vec3f getLight(
			const Vec3f& pos,
			const Vec3f& normal,
			const Camera& camera,
			unsigned specularExponent,
			float specularIntensity
		) const override;
	};


	class LightingList {
	protected:
		const Vec3f& ambientLight;
		const std::vector<Light*>& lights;
	public:
		LightingList(
			const Vec3f& ambientLight,
			const std::vector<Light*>& lights
		) : 
			ambientLight(ambientLight),
			lights(lights)
		{}
		virtual Vec3f getAllLight(
			const Vec3f& pos,
			const Vec3f& normal
		) const;
	};

	class LightingListSpecular : public LightingList {
		const Camera& camera;
		uint16_t specularExponent;
		float specularIntensity;
	public:
		LightingListSpecular(
			const Vec3f& ambientLight,
			const std::vector<Light*>& lights,
			const Camera& camera,
			uint16_t specularExponent,
			float specularIntensity
		) :
			LightingList(ambientLight, lights),
			camera(camera),
			specularExponent(specularExponent),
			specularIntensity(specularIntensity)
		{}
		virtual Vec3f getAllLight(
			const Vec3f& pos,
			const Vec3f& normal
		) const override;
	};

}
