#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include "vec.h"

namespace bl {

	class Light {
	protected:
		Vec3f color;
	public:
		Light(Vec3f& color);
		virtual void getLight(Vertex& v) const = 0;
		virtual void move(const Vec3f& pos) = 0;
	};

	class Light_Dir : public Light {
		Vec3f dir;
	public:
		Light_Dir(Vec3f& color, Vec3f& dir);
		virtual void getLight(Vertex& v) const override;
		virtual void move(const Vec3f& pos) override;
	};

	class Light_Pt : public Light {
		Vec3f pos;
		float falloff;
	public:
		Light_Pt(Vec3f& color, Vec3f& pos, float falloff);
		virtual void getLight(Vertex& v) const override;
		void move(const Vec3f& pos) override;
	};

	int rgbToDec(Vec3f rgb);

}

#endif
