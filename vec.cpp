#include <cmath>
#include "vec.h"

namespace bl {

	Vec3f& Vec3f::operator+=(float num) {
		x += num;
		y += num;
		z += num;
		return *this;
	}

	Vec3f& Vec3f::operator-=(float num) {
		x -= num;
		y -= num;
		z -= num;
		return *this;
	}

	Vec3f& Vec3f::operator*=(float num) {
		x *= num;
		y *= num;
		z *= num;
		return *this;
	}

	Vec3f& Vec3f::operator/=(float num) {
		x /= num;
		y /= num;
		z /= num;
		return *this;
	}


	Vec3f Vec3f::operator+(float num) const {
		Vec3f sum;
		sum.x = x + num;
		sum.y = y + num;
		sum.z = z + num;
		return sum;
	}

	Vec3f Vec3f::operator-(float num) const {
		Vec3f diff;
		diff.x = x - num;
		diff.y = y - num;
		diff.z = z - num;
		return diff;
	}

	Vec3f Vec3f::operator*(float num) const {
		Vec3f prod;
		prod.x = x * num;
		prod.y = y * num;
		prod.z = z * num;
		return prod;
	}

	Vec3f Vec3f::operator/(float num) const {
		Vec3f quot;
		quot.x = x / num;
		quot.y = y / num;
		quot.z = z / num;
		return quot;
	}


	Vec3f& Vec3f::operator+=(const Vec3f& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vec3f& Vec3f::operator-=(const Vec3f& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vec3f& Vec3f::operator*=(const Vec3f& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		return *this;
	}

	Vec3f& Vec3f::operator/=(const Vec3f& vec) {
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		return *this;
	}

	Vec3f Vec3f::operator+(const Vec3f& vec) const {
		Vec3f sum;
		sum.x = x + vec.x;
		sum.y = y + vec.y;
		sum.z = z + vec.z;
		return sum;
	}

	Vec3f Vec3f::operator-(const Vec3f& vec) const {
		Vec3f diff;
		diff.x = x - vec.x;
		diff.y = y - vec.y;
		diff.z = z - vec.z;
		return diff;
	}

	Vec3f Vec3f::operator*(const Vec3f& vec) const {
		Vec3f prod;
		prod.x = x * vec.x;
		prod.y = y * vec.y;
		prod.z = z * vec.z;
		return prod;
	}

	Vec3f Vec3f::operator/(const Vec3f& vec) const {
		Vec3f quot;
		quot.x = x / vec.x;
		quot.y = y / vec.y;
		quot.z = z / vec.z;
		return quot;
	}

	Vec3f& Vec3f::operator=(float num) {
		x = num;
		y = num;
		z = num;
		return *this;
	}

	float Vec3f::getMagnitude() const {
		return sqrtf(x*x + y*y + z*z);
	}

	Vec3f Vec3f::getNormalized() const {
		return *this / getMagnitude();
	}

	float dotProduct(const Vec3f& vec1, const Vec3f& vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2) {
		Vec3f out;
		out.x = vec1.y * vec2.z - vec1.z * vec2.y;
		out.y = vec1.z * vec2.x - vec1.x * vec2.z;
		out.z = vec1.x * vec2.y - vec1.y * vec2.x;
		return out;
	}

	float dirSimilarity(const Vec3f& vec1, const Vec3f& vec2) {
		Vec3f vec1n = vec1.getNormalized();
		Vec3f vec2n = vec2.getNormalized();
		float sim = (dotProduct(vec1n, vec2n) + 1.0f) / 2.0f;
		return sim;
	}

}
