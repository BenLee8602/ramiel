#pragma once

#include <fstream>
#include <cmath>
#include <vector>

namespace ramiel {

    template<typename T, size_t N>
    struct Vec {
        T vec[N];

        // element retrieval
        T& operator[](size_t index) { return vec[index]; }
        const T& operator[](size_t index) const { return vec[index]; }

        // typecasting
        explicit operator T*() { return vec; }
        operator bool() const { for (size_t i = 0; i < N; ++i) if (vec[i]) return true; return false; }
        template<typename U> explicit operator Vec<U, N>() const { Vec<U, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = (U)vec[i]; return temp; }

        // comparison
        template<typename U> bool operator==(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (vec[i] != v[i]) return false; return true; }
        template<typename U> bool equals(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (std::abs(vec[i] - v[i]) > (T)0.001) return false; return true; }

        // assignment
        template<typename U> Vec<T, N>& operator=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) vec[i] = v[i]; return *this; }
        template<typename U> Vec<T, N>& operator=(U num) { for (size_t i = 0; i < N; ++i) vec[i] = num; return *this; }

        // scalar arithmetic assignment
        template<typename U> Vec<T, N>& operator+=(U num) { for (size_t i = 0; i < N; ++i) vec[i] += num; return *this; }
        template<typename U> Vec<T, N>& operator-=(U num) { for (size_t i = 0; i < N; ++i) vec[i] -= num; return *this; }
        template<typename U> Vec<T, N>& operator*=(U num) { for (size_t i = 0; i < N; ++i) vec[i] *= num; return *this; }
        template<typename U, typename F = float> Vec<T, N>& operator/=(U num) {
            F inum = (F)1 / num;
            for (size_t i = 0; i < N; ++i) vec[i] *= inum;
            return *this;
        }

        // vector arithmetic assigment
        template<typename U> Vec<T, N>& operator+=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) vec[i] += v[i]; return *this; }
        template<typename U> Vec<T, N>& operator-=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) vec[i] -= v[i]; return *this; }
        template<typename U> Vec<T, N>& operator*=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) vec[i] *= v[i]; return *this; }
        template<typename U> Vec<T, N>& operator/=(const Vec<U, N>& v) { for (size_t i = 0; i < N; ++i) vec[i] /= v[i]; return *this; }

        // scalar arithmetic
        template<typename U> Vec<T, N> operator+(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] + num; return temp; }
        template<typename U> Vec<T, N> operator-(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] - num; return temp; }
        template<typename U> Vec<T, N> operator*(U num) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] * num; return temp; }
        template<typename U, typename F = float> Vec<T, N> operator/(U num) const {
            Vec<T, N> temp;
            F inum = (F)1 / num;
            for (size_t i = 0; i < N; ++i) temp[i] = vec[i] * inum;
            return temp;
        }

        // vector arithmetic
        template<typename U> Vec<T, N> operator+(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] + v[i]; return temp; }
        template<typename U> Vec<T, N> operator-(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] - v[i]; return temp; }
        template<typename U> Vec<T, N> operator*(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] * v[i]; return temp; }
        template<typename U> Vec<T, N> operator/(const Vec<U, N>& v) const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = vec[i] / v[i]; return temp; }

        // negate
        Vec<T, N> operator-() const { Vec<T, N> temp; for (size_t i = 0; i < N; ++i) temp[i] = -vec[i]; return temp; }
    };

    template<typename T, size_t N>
    std::ostream& operator<<(std::ostream& os, const Vec<T, N>& vec) {
        os << "{ ";
        for (size_t i = 0; i < N - 1; ++i) {
            os << vec[i] << ", ";
        }
        return os << vec[N - 1] << " }";
    }


    enum xyz : size_t { X, Y, Z };
    enum rgb : size_t { R, G, B };
    enum uv  : size_t { U, V };

    typedef Vec<char, 2> Vec2i8;
    typedef Vec<char, 3> Vec3i8;
    typedef Vec<unsigned char, 2> Vec2ui8;
    typedef Vec<unsigned char, 3> Vec3ui8;

    typedef Vec<short, 2> Vec2i16;
    typedef Vec<short, 3> Vec3i16;
    typedef Vec<unsigned short, 2> Vec2ui16;
    typedef Vec<unsigned short, 3> Vec3ui16;

    typedef Vec<int, 2> Vec2;
    typedef Vec<int, 3> Vec3;
    typedef Vec<unsigned int, 2> Vec2u;
    typedef Vec<unsigned int, 3> Vec3u;

    typedef Vec<long, 2> Vec2i32;
    typedef Vec<long, 3> Vec3i32;
    typedef Vec<unsigned long, 2> Vec2ui32;
    typedef Vec<unsigned long, 3> Vec3ui32;

    typedef Vec<long long, 2> Vec2i64;
    typedef Vec<long long, 3> Vec3i64;
    typedef Vec<unsigned long long, 2> Vec2ui64;
    typedef Vec<unsigned long long, 3> Vec3ui64;

    typedef Vec<float, 2> Vec2f;
    typedef Vec<float, 3> Vec3f;
    typedef Vec<double, 2> Vec2lf;
    typedef Vec<double, 3> Vec3lf;


    template<typename T, size_t N>
    T dot(const Vec<T, N>& v1, const Vec<T, N>& v2) {
        T d = (T)0;
        for (size_t i = 0; i < N; ++i)
            d += v1[i] * v2[i];
        return d;
    }

    template<typename T, size_t N>
    Vec<T, N> cross(const Vec<T, N>& vec1, const Vec<T, N>& vec2) {
        Vec<T, N> c{};
        c[X] = vec1[Y] * vec2[Z] - vec1[Z] * vec2[Y];
        c[Y] = vec1[Z] * vec2[X] - vec1[X] * vec2[Z];
        c[Z] = vec1[X] * vec2[Y] - vec1[Y] * vec2[X];
        return c;
    }

    template<typename T, size_t N>
    T mag(const Vec<T, N>& v) {
        return std::sqrt(dot(v, v));
    }

    template<typename T, size_t N>
    Vec<T, N> normalize(const Vec<T, N>& v) {
        return v / mag(v);
    }

    template<typename T, size_t N>
    Vec<T, N> normalize(const Vec<T, N>& v, const T& mag) {
        return v / mag;
    }

    template<typename T, size_t N>
    Vec<T, N> min(const Vec<T, N>& v, const Vec<T, N>& min_) {
        Vec<T, N> out;
        for (size_t i = 0; i < N; ++i)
            out[i] = std::min(v[i], min_[i]);
        return out;
    }

    template<typename T, size_t N>
    Vec<T, N> max(const Vec<T, N>& v, const Vec<T, N>& max_) {
        Vec<T, N> out;
        for (size_t i = 0; i < N; ++i)
            out[i] = std::max(v[i], max_[i]);
        return out;
    }

    template<typename T, size_t N>
    Vec<T, N> min(const Vec<T, N>& v, const T& min_) {
        Vec<T, N> out;
        for (size_t i = 0; i < N; ++i)
            out[i] = std::min(v[i], min_);
        return out;
    }

    template<typename T, size_t N>
    Vec<T, N> max(const Vec<T, N>& v, const T& max_) {
        Vec<T, N> out;
        for (size_t i = 0; i < N; ++i)
            out[i] = std::max(v[i], max_);
        return out;
    }

}
