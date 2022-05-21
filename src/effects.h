#pragma once

#include <memory>
#include "vec.h"

namespace ramiel {

	class Effect {
		bool enabled;
	public:
		Effect(bool enabled) : enabled(enabled) {}
		bool isEnabled() const;
		void enable(bool enabled = true);
		virtual void applyEffect(Vec3f* in, Vec3f* out) const = 0;
	};

	class ColorShift : public Effect {
		Vec3f color;
		float strength;
	public:
		ColorShift(Vec3f color, float strength, bool enabled = false);
		const Vec3f& getColor() const;
		float getStrength() const;
		void setColor(Vec3f color);
		void setStrength(float strength);
		virtual void applyEffect(Vec3f* in, Vec3f* out) const override;
	};

	class Fog : public Effect {
		float start;
		float end;
		float fac;
		Vec3f color;
	public:
		Fog(float start, float end, Vec3f color, bool enabled = false);
		float getStart() const;
		float getEnd() const;
		const Vec3f& getColor() const;
		void setStart(float start);
		void setEnd(float end);
		void setColor(Vec3f color);
		virtual void applyEffect(Vec3f* in, Vec3f* out) const override;
	};

	class Greyscale : public Effect {
	public:
		Greyscale(bool enabled = false) : Effect(enabled) {}
		virtual void applyEffect(Vec3f* in, Vec3f* out) const override;
	};

	class Blur : public Effect {
		unsigned rad;
	public:
		Blur(unsigned rad, bool enabled = false) : Effect(enabled), rad(rad) {};
		unsigned getRad() const;
		void setRad(unsigned rad);
		virtual void applyEffect(Vec3f* in, Vec3f* out) const override;
	};

	class Bloom : public Blur {
	public:
		Bloom(unsigned rad = 25, bool enabled = false) : Blur(rad, enabled) {};
		virtual void applyEffect(Vec3f* in, Vec3f* out) const override;
	};

}
