#pragma once

#include <cmath>
#include <iostream>

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

        // scalar comparison
        template<typename U> bool operator==(const U& num) const { for (size_t i = 0; i < N; ++i) if (vec[i] != num) return false; return true; }
        template<typename U> bool operator<=(const U& num) const { for (size_t i = 0; i < N; ++i) if (vec[i] > num) return false; return true; }
        template<typename U> bool operator>=(const U& num) const { for (size_t i = 0; i < N; ++i) if (vec[i] < num) return false; return true; }
        template<typename U> bool operator<(const U& num) const { for (size_t i = 0; i < N; ++i) if (vec[i] >= num) return false; return true; }
        template<typename U> bool operator>(const U& num) const { for (size_t i = 0; i < N; ++i) if (vec[i] <= num) return false; return true; }

        // vector comparison
        template<typename U> bool operator==(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (vec[i] != v[i]) return false; return true; }
        template<typename U> bool operator<=(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (vec[i] > v[i]) return false; return true; }
        template<typename U> bool operator>=(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (vec[i] < v[i]) return false; return true; }
        template<typename U> bool operator<(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (vec[i] >= v[i]) return false; return true; }
        template<typename U> bool operator>(const Vec<U, N>& v) const { for (size_t i = 0; i < N; ++i) if (vec[i] <= v[i]) return false; return true; }

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


    template<typename T, size_t N, typename U>
    bool equal(const Vec<T, N>& v1, const U& v2, float thres = 0.0001) {
        Vec<T, N> v = v1 - v2;
        return v <= thres && v >= -thres;
    }


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
        c[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
        c[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
        c[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
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


    enum xyz : size_t { X, Y, Z, W };
    enum rgb : size_t { R, G, B };
    enum uv  : size_t { U, V };

    typedef Vec<int, 2> Vec2;
    typedef Vec<int, 3> Vec3;
    typedef Vec<int, 4> Vec4;
    typedef Vec<unsigned, 2> Vec2u;
    typedef Vec<unsigned, 3> Vec3u;
    typedef Vec<unsigned, 4> Vec4u;

    typedef Vec<float, 2> Vec2f;
    typedef Vec<float, 3> Vec3f;
    typedef Vec<float, 4> Vec4f;
    typedef Vec<double, 2> Vec2lf;
    typedef Vec<double, 3> Vec3lf;
    typedef Vec<double, 4> Vec4lf;

    typedef Vec<int8_t, 2> Vec2i8;
    typedef Vec<int8_t, 3> Vec3i8;
    typedef Vec<int8_t, 4> Vec4i8;
    typedef Vec<uint8_t, 2> Vec2ui8;
    typedef Vec<uint8_t, 3> Vec3ui8;
    typedef Vec<uint8_t, 4> Vec4ui8;

    typedef Vec<int16_t, 2> Vec2i16;
    typedef Vec<int16_t, 3> Vec3i16;
    typedef Vec<int16_t, 4> Vec4i16;
    typedef Vec<uint16_t, 2> Vec2ui16;
    typedef Vec<uint16_t, 3> Vec3ui16;
    typedef Vec<uint16_t, 4> Vec4ui16;

    typedef Vec<int32_t, 2> Vec2i32;
    typedef Vec<int32_t, 3> Vec3i32;
    typedef Vec<int32_t, 4> Vec4i32;
    typedef Vec<uint32_t, 2> Vec2ui32;
    typedef Vec<uint32_t, 3> Vec3ui32;
    typedef Vec<uint32_t, 4> Vec4ui32;

    typedef Vec<int64_t, 2> Vec2i64;
    typedef Vec<int64_t, 3> Vec3i64;
    typedef Vec<int64_t, 4> Vec4i64;
    typedef Vec<uint64_t, 2> Vec2ui64;
    typedef Vec<uint64_t, 3> Vec3ui64;
    typedef Vec<uint64_t, 4> Vec4ui64;

}
