#pragma once

#include <fstream>
#include <cmath>
#include <vector>

namespace ramiel {

	template<typename T, size_t N>
	struct Vec {
		T arr[N];

		// element retrieval
		T& operator[](size_t index) { return arr[index]; }
		const T& operator[](size_t index) const { return arr[index]; }

		// typecasting
		explicit operator T* () { return arr; }
		operator bool() const { for (size_t i = 0; i < N; ++i) if (arr[i]) return true; return false; }
		template<typename U> explicit operator Vec<U, N>() const { Vec<U, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = (U)arr[i]; return temp; }

		// comparison
		template<typename U> bool operator==(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (arr[i] != v[i]) return false; return true; }
		template<typename U> bool equals(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (std::abs(arr[i] - v[i]) > (T)0.001) return false; return true; }

		// assignment
		template<typename U> Vec<T, N>& operator=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) arr[i] = v[i]; return *this; }
		template<typename U> Vec<T, N>& operator=(U num) { for (size_t i = 0; i < N; ++i) arr[i] = num; return *this; }

		// scalar arithmetic assignment
		template<typename U> Vec<T, N>& operator+=(U num) { for (size_t i = 0; i < N; ++i) arr[i] += num; return *this; }
		template<typename U> Vec<T, N>& operator-=(U num) { for (size_t i = 0; i < N; ++i) arr[i] -= num; return *this; }
		template<typename U> Vec<T, N>& operator*=(U num) { for (size_t i = 0; i < N; ++i) arr[i] *= num; return *this; }
		template<typename U, typename F = float> Vec<T, N>& operator/=(U num) {
			F inum = (F)1 / num;
			for (size_t i = 0; i < N; ++i) arr[i] *= inum;
			return *this;
		}

		// vector arithmetic assigment
		template<typename U> Vec<T, N>& operator+=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) arr[i] += v[i]; return *this; }
		template<typename U> Vec<T, N>& operator-=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) arr[i] -= v[i]; return *this; }
		template<typename U> Vec<T, N>& operator*=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) arr[i] *= v[i]; return *this; }
		template<typename U> Vec<T, N>& operator/=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) arr[i] /= v[i]; return *this; }

		// scalar arithmetic
		template<typename U> Vec<T, N> operator+(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] + num; return temp; }
		template<typename U> Vec<T, N> operator-(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] - num; return temp; }
		template<typename U> Vec<T, N> operator*(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] * num; return temp; }
		template<typename U, typename F = float> Vec<T, N> operator/(U num) const {
			Vec<T, N> temp;
			F inum = (F)1 / num;
			for (size_t i = 0; i < N; ++i) temp[i] = arr[i] * inum;
			return temp;
		}

		// vector arithmetic
		template<typename U> Vec<T, N> operator+(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] + v[i]; return temp; }
		template<typename U> Vec<T, N> operator-(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] - v[i]; return temp; }
		template<typename U> Vec<T, N> operator*(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] * v[i]; return temp; }
		template<typename U> Vec<T, N> operator/(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = arr[i] / v[i]; return temp; }

		// negate
		Vec<T, N> operator-() const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = -arr[i]; return temp; }
	};

	template<typename T, size_t N>
	std::ostream& operator<<(std::ostream& os, const Vec<T, N>& vec) {
		os << "{ ";
		for (size_t i = 0; i < N - 1; ++i) {
			os << vec[i] << ", ";
		}
		return os << vec[N - 1] << " }";
	}


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
	enum uv  : size_t { U, V };

	constexpr Vec2  vec2_0  = { 0, 0 };
	constexpr Vec3  vec3_0  = { 0, 0, 0 };
	constexpr Vec2u vec2u_0 = { 0u, 0u };
	constexpr Vec3u vec3u_0 = { 0u, 0u, 0u };
	constexpr Vec2f vec2f_0 = { 0.0f, 0.0f };
	constexpr Vec3f vec3f_0 = { 0.0f, 0.0f, 0.0f };
	constexpr Vec3f vec3f_255 = { 255.0f, 255.0f, 255.0f };


	float getMagnitude(const Vec3f& v);
	Vec3f getNormalized(const Vec3f& v);
	Vec3f getNormalized(const Vec3f& v, float magnitude);
	float dotProduct(const Vec3f& vec1, const Vec3f& vec2);
	Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2);


	void c_min(Vec3f& color, const Vec3f& min = vec3f_255);
	void c_max(Vec3f& color, const Vec3f& max = vec3f_0);
	void c_clamp(Vec3f& color);

}
