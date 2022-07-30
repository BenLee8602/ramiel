#pragma once

#include "vec.h"

namespace ramiel {

	class Light {
	protected:
	#ifdef RAMIEL_TEST
	public:
	#endif
		Vec3f color;
		float intensity;
	public:
		Light(Vec3f color, float intensity);
		virtual Vec3f getLight(
			const Vec3f& pos, const Vec3f& normal,
			unsigned specularExponent, float specularIntensity
		) const = 0;
		virtual void move(const Vec3f& pos) = 0;
	};

	class Light_Dir : public Light {
	protected:
	#ifdef RAMIEL_TEST
	public:
	#endif
		Vec3f dir;
	public:
		Light_Dir(Vec3f color, float intensity, Vec3f dir);
		virtual Vec3f getLight(
			const Vec3f& pos, const Vec3f& normal,
			unsigned specularExponent, float specularIntensity
		) const override;
		virtual void move(const Vec3f& pos) override;
	};

	class Light_Pt : public Light {
	protected:
	#ifdef RAMIEL_TEST
	public:
	#endif
		Vec3f pos;
		float falloff;
	public:
		Light_Pt(Vec3f color, float intensity, Vec3f pos, float falloff);
		virtual Vec3f getLight(
			const Vec3f& pos, const Vec3f& normal,
			unsigned specularExponent, float specularIntensity
		) const override;
		virtual void move(const Vec3f& pos) override;
	};

	class Light_Sp : public Light_Pt {
	#ifdef RAMIEL_TEST
	public:
	#endif
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
			const Vec3f& pos, const Vec3f& normal,
			unsigned specularExponent, float specularIntensity
		) const override;
		void rotate(const Vec3f& dir);
	};

}
