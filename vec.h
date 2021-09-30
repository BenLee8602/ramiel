#ifndef BL_VEC_H
#define BL_VEC_H

#include <fstream>

namespace bl {

	struct Vec2 {
		int x;
		int y;

		// scalar arithmetic assignment
		Vec2& operator+=(int num);
		Vec2& operator-=(int num);
		Vec2& operator*=(int num);
		Vec2& operator/=(int num);

		// scalar arithmetic
		Vec2 operator+(int num) const;
		Vec2 operator-(int num) const;
		Vec2 operator*(int num) const;
		Vec2 operator/(int num) const;

		// vector arithmetic assigment
		Vec2& operator+=(const Vec2& vec);
		Vec2& operator-=(const Vec2& vec);
		Vec2& operator*=(const Vec2& vec);
		Vec2& operator/=(const Vec2& vec);

		// vector arithmetic
		Vec2 operator+(const Vec2& vec) const;
		Vec2 operator-(const Vec2& vec) const;
		Vec2 operator*(const Vec2& vec) const;
		Vec2 operator/(const Vec2& vec) const;

		Vec2& operator=(int num);
		operator bool();

		float getMagnitude() const;
		Vec2 getNormalized() const;
	};


	struct Vec3 {
		int x;
		int y;
		int z;
	};


	struct Vec2f {
		float x;
		float y;
	};


	struct Vec3f {
		float x;
		float y;
		float z;

		// scalar arithmetic assignment
		Vec3f& operator+=(float num);
		Vec3f& operator-=(float num);
		Vec3f& operator*=(float num);
		Vec3f& operator/=(float num);

		// scalar arithmetic
		Vec3f operator+(float num) const;
		Vec3f operator-(float num) const;
		Vec3f operator*(float num) const;
		Vec3f operator/(float num) const;
		
		// vector arithmetic assigment
		Vec3f& operator+=(const Vec3f& vec);
		Vec3f& operator-=(const Vec3f& vec);
		Vec3f& operator*=(const Vec3f& vec);
		Vec3f& operator/=(const Vec3f& vec);

		// vector arithmetic
		Vec3f operator+(const Vec3f& vec) const;
		Vec3f operator-(const Vec3f& vec) const;
		Vec3f operator*(const Vec3f& vec) const;
		Vec3f operator/(const Vec3f& vec) const;

		Vec3f& operator=(float num);
		operator bool();

		float getMagnitude() const;
		Vec3f getNormalized() const;
	};

	float dotProduct(const Vec3f& vec1, const Vec3f& vec2);
	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2);
	float dirSimilarity(const Vec3f& vec1, const Vec3f& vec2);
	// similarity of direction of 2 vectors, 
	// 0.0f meaning opposite, 1.0f meaning same direction

	std::ostream& operator<<(std::ostream& os, const Vec3f& vec);

	struct Vertex {
		Vec3f pos = { 0.0f };
		Vec3f normal = { 0.0f };
		Vec3f color = { 0.0f };
	};

}

#endif
