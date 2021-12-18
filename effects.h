#ifndef BL_EFFECTS_H
#define BL_EFFECTS_H

#include <memory>
#include "vec.h"
#undef min
#undef max

namespace bl {

	class Effect {
		bool enabled;
	public:
		Effect(bool enabled) : enabled(enabled) {}
		bool isEnabled() const;
		void enable(bool enabled = true);
		virtual void applyEffect() const = 0;
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
		virtual void applyEffect() const override;
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
		virtual void applyEffect() const override;
	};

	class Greyscale : public Effect {
	public:
		Greyscale(bool enabled = false) : Effect(enabled) {}
		virtual void applyEffect() const override;
	};

	class Blur : public Effect {
		int rad;
	public:
		Blur(int rad, bool enabled = false) : Effect(enabled), rad(rad) {};
		virtual void applyEffect() const override;
	};

}

#endif
