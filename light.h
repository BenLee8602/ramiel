#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include "vec.h"

namespace bl {

	class Light {
	protected:
		Vec3f color;
	public:
		Light(Vec3f color);
		virtual void getLight(Vertex& v) const = 0;
		virtual void move(const Vec3f& pos) = 0;
	};

	class Light_Dir : public Light {
	protected:
		Vec3f dir;
	public:
		Light_Dir(
			Vec3f color = vec3f_255,
			Vec3f dir = { 0.0f, 0.0f, -1.0f }
		);
		virtual void getLight(Vertex& v) const override;
		virtual void move(const Vec3f& pos) override;
	};

	class Light_Pt : public Light {
	protected:
		Vec3f pos;
		float falloff;
	public:
		Light_Pt(
			Vec3f color = vec3f_255,
			Vec3f pos = vec3f_0,
			float falloff = 1.0f
		);
		virtual void getLight(Vertex& v) const override;
		virtual void move(const Vec3f& pos) override;
	};

	class Light_Sp : public Light_Pt {
		Vec3f dir;
		float width;
		float falloffExp;
	public:
		Light_Sp(
			Vec3f color = vec3f_255,
			Vec3f pos = vec3f_0,
			Vec3f dir = { 0.0f, 0.0f, 1.0f },
			float falloff = 0.1f,
			float width = 30.0f,
			float falloffExp = 50.0f
		);
		virtual void getLight(Vertex& v) const override;
		void rotate(const Vec3f& dir);
	};

}

#endif
