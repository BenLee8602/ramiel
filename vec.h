#ifndef BL_VEC_H
#define BL_VEC_H

#include <fstream>
#include <cmath>

namespace bl {

	template<typename T, size_t N>
	struct Vec {
		T arr[N];
		T& operator[](size_t index) { return arr[index]; }
		const T& operator[](size_t index) const { return arr[index]; }

		// scalar arithmetic assignment
		template<typename U> Vec<T, N>& operator+=(U num) { for (size_t i = 0; i < N; i++) arr[i] += num; return *this; }
		template<typename U> Vec<T, N>& operator-=(U num) { for (size_t i = 0; i < N; i++) arr[i] -= num; return *this; }
		template<typename U> Vec<T, N>& operator*=(U num) { for (size_t i = 0; i < N; i++) arr[i] *= num; return *this; }
		template<typename U> Vec<T, N>& operator/=(U num) { for (size_t i = 0; i < N; i++) arr[i] /= num; return *this; }

		// vector arithmetic assigment
		template<typename U> Vec<T, N>& operator+=(const Vec<U, N>& v) { for (size_t i = 0; i < N; i++) arr[i] += v[i]; return *this; }
		template<typename U> Vec<T, N>& operator-=(const Vec<U, N>& v) { for (size_t i = 0; i < N; i++) arr[i] -= v[i]; return *this; }
		template<typename U> Vec<T, N>& operator*=(const Vec<U, N>& v) { for (size_t i = 0; i < N; i++) arr[i] *= v[i]; return *this; }
		template<typename U> Vec<T, N>& operator/=(const Vec<U, N>& v) { for (size_t i = 0; i < N; i++) arr[i] /= v[i]; return *this; }

		// scalar arithmetic
		template<typename U> Vec<T, N> operator+(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] + num; return temp; }
		template<typename U> Vec<T, N> operator-(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] - num; return temp; }
		template<typename U> Vec<T, N> operator*(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] * num; return temp; }
		template<typename U> Vec<T, N> operator/(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] / num; return temp; }

		// vector arithmetic
		template<typename U> Vec<T, N> operator+(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] + v[i]; return temp; }
		template<typename U> Vec<T, N> operator-(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] - v[i]; return temp; }
		template<typename U> Vec<T, N> operator*(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] * v[i]; return temp; }
		template<typename U> Vec<T, N> operator/(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; i++) temp[i] = arr[i] / v[i]; return temp; }

		template<typename U> Vec<T, N>& operator=(U num) { for (size_t i = 0; i < N; i++) arr[i] = num; return *this; }
	};


	typedef Vec<int, 2> Vec2;
	typedef Vec<int, 3> Vec3;
	typedef Vec<float, 2> Vec2f;
	typedef Vec<float, 3> Vec3f;

	enum xyz : size_t { x, y, z };
	enum rgb : size_t { r, g, b };


	float getMagnitude(const Vec3f& v);
	Vec3f getNormalized(const Vec3f& v);
	float dotProduct(const Vec3f& vec1, const Vec3f& vec2);
	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2);
	float dirSimilarity(const Vec3f& vec1, const Vec3f& vec2);
	std::ostream& operator<<(std::ostream& os, const Vec3f& vec);


	void c_min(Vec3f& color);
	void c_max(Vec3f& color);
	void c_clamp(Vec3f& color);
	int rgbToDec(const Vec3f& color);


	struct Vertex {
		Vec3f pos = { 0.0f };
		Vec3f normal = { 0.0f };
		Vec3f color = { 0.0f };
	};

}

#endif
