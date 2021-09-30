#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include "vec.h"

namespace bl {

	struct RGB {
		uint16_t r;
		uint16_t g;
		uint16_t b;
		
		int getDec();
		void setDec(int dec);

		void clamp();

		// scalar arithmetic assignment
		RGB& operator+=(uint16_t num);
		RGB& operator-=(uint16_t num);
		RGB& operator*=(uint16_t num);
		RGB& operator/=(uint16_t num);

		// scalar arithmetic
		RGB operator+(uint16_t num) const;
		RGB operator-(uint16_t num) const;
		RGB operator*(uint16_t num) const;
		RGB operator/(uint16_t num) const;

		// rgb arithmetic assigment
		RGB& operator+=(const RGB& other);
		RGB& operator-=(const RGB& other);
		RGB& operator*=(const RGB& other);
		RGB& operator/=(const RGB& other);

		// rgb arithmetic
		RGB operator+(const RGB& other) const;
		RGB operator-(const RGB& other) const;
		RGB operator*(const RGB& other) const;
		RGB operator/(const RGB& other) const;
	};

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
