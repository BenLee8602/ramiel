#ifndef BL_VEC_H
#define BL_VEC_H

#include <fstream>

namespace bl {

	struct Vec2 {
		int x;
		int y;
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

}

#endif
