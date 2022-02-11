#ifndef BL_VEC_H
#define BL_VEC_H

#include <fstream>
#include <cmath>

namespace bl {

	template<typename T, size_t N>
	struct Vec {
		T arr[N];

		// element retrieval
		T& operator[](size_t index) { return arr[index]; }
		const T& operator[](size_t index) const { return arr[index]; }

		// typecasting
		operator T* () { return arr; };
		operator bool() const { for (size_t i = 0; i < N; i++) if (arr[i]) return true; return false; }

		// assignment
		template<typename U> Vec<T, N>& operator=(const Vec<U, N>& v) { for (size_t i = 0; i < N; i++) arr[i] = v[i]; return *this; }
		template<typename U> Vec<T, N>& operator=(U num) { for (size_t i = 0; i < N; i++) arr[i] = num; return *this; }

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
	};


	typedef Vec<char, 2> Vec2i8;			typedef Vec<unsigned char, 2> Vec2ui8;
	typedef Vec<char, 3> Vec3i8;			typedef Vec<unsigned char, 3> Vec3ui8;

	typedef Vec<short, 2> Vec2i16;			typedef Vec<unsigned short, 2> Vec2ui16;
	typedef Vec<short, 3> Vec3i16;			typedef Vec<unsigned short, 3> Vec3ui16;

	typedef Vec<int, 2> Vec2;				typedef Vec<unsigned int, 2> Vec2u;
	typedef Vec<int, 3> Vec3;				typedef Vec<unsigned int, 3> Vec3u;

	typedef Vec<long, 2> Vec2i32;			typedef Vec<unsigned long, 2> Vec2ui32;
	typedef Vec<long, 3> Vec3i32;			typedef Vec<unsigned long, 3> Vec3ui32;
	
	typedef Vec<long long, 2> Vec2i64;		typedef Vec<unsigned long long, 2> Vec2ui64;
	typedef Vec<long long, 3> Vec3i64;		typedef Vec<unsigned long long, 3> Vec3ui64;
	
	typedef Vec<float, 2> Vec2f;			typedef Vec<double, 2> Vec2lf;
	typedef Vec<float, 3> Vec3f;			typedef Vec<double, 3> Vec3lf;


	enum xyz : size_t { X, Y, Z };
	enum rgb : size_t { R, G, B };

	const Vec3f vec3f_0 = { 0.0f, 0.0f, 0.0f };
	const Vec3f vec3f_255 = { 255.0f, 255.0f, 255.0f };


	float getMagnitude(const Vec3f& v);
	Vec3f getNormalized(const Vec3f& v);
	float dotProduct(const Vec3f& vec1, const Vec3f& vec2);
	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2);
	std::ostream& operator<<(std::ostream& os, const Vec3f& vec);


	void c_min(Vec3f& color, const Vec3f& min = vec3f_255);
	void c_max(Vec3f& color, const Vec3f& max = vec3f_0);
	void c_clamp(Vec3f& color);
	int rgbToDec(const Vec3f& color);
	void notBloom(Vec3f& in);


	struct Vertex {
		Vec3f pos = { 0.0f };
		Vec3f normal = { 0.0f };
		Vec3f color = { 0.0f };
	};

}

#endif
