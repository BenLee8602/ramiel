#include "vec.h"

namespace ramiel {

    template<typename T, size_t M, size_t N>
    Vec<T, N> matvec(const Vec<Vec<T, M>, N>& mat, const Vec<T, M>& in) {
        Vec<T, N> out;
        for (size_t i = 0; i < N; ++i)
            out[i] = dot(mat[i], in);
        return out;
    }


    template<typename T, size_t M, size_t N, size_t P>
    Vec<Vec<T, M>, P> matmat(const Vec<Vec<T, M>, N>& mat2, const Vec<Vec<T, N>, P>& mat1) {
        Vec<Vec<T, M>, P> mat{};
        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < P; ++j) {
                for (size_t k = 0; k < N; ++k) {
                    mat[j][i] += mat2[k][i] * mat1[j][k];
                }
            }
        }
        return mat;
    }


    template<typename T, size_t N>
    constexpr Vec<Vec<T, N>, N> id() {
        Vec<Vec<T, N>, N> mat{};
        for (size_t i = 0; i < N; ++i)
            mat[i][i] = (T)1;
        return mat;
    }


    template<typename T, size_t M, size_t N>
    Vec<Vec<T, N>, M> tp(const Vec<Vec<T, M>, N>& in) {
        Vec<Vec<T, N>, M> out;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                out[j][i] = in[i][j];
            }
        }
        return out;
    }


    template<typename T, size_t M, size_t N>
    Vec<Vec<T, M - 1>, N - 1> sub(const Vec<Vec<T, M>, N>& in, size_t i, size_t j) {
        Vec<Vec<T, M - 1>, N - 1> out;
        for (size_t k = 0; k < i; ++k) {
            for (size_t l = 0; l < j; ++l)
                out[k][l] = in[k][l];
            for (size_t l = j + 1; l < M; ++l)
                out[k][l - 1] = in[k][l];
        }
        for (size_t k = i + 1; k < N; ++k) {
            for (size_t l = 0; l < j; ++l)
                out[k - 1][l] = in[k][l];
            for (size_t l = j + 1; l < M; ++l)
                out[k - 1][l - 1] = in[k][l];
        }
        return out;
    }


    template<typename T, size_t N>
    T det(const Vec<Vec<T, N>, N>& in) {
        static_assert(N < 10, "cant calculate det for given matrix dims");
        if constexpr (N == 1) return in[0][0];
        else if constexpr (N == 2) {
            return in[0][0] * in[1][1] - in[0][1] * in[1][0];
        } else {
            T out = (T)0;
            for (size_t i = 0; i < N; ++i) {
                T coeff = in[0][i] * (T)(i % 2 ? -1 : 1);
                out += coeff * det(sub(in, 0, i));
            }
            return out;
        }
    }


    template<typename T, size_t N>
    Vec<Vec<T, N>, N> inv(const Vec<Vec<T, N>, N>& mat) {
        T d = det(mat);
        if (d == 0) return Vec<Vec<T, N>, N>();

        Vec<Vec<T, N>, N> cofac;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                T minor = det(sub(mat, i, j));
                T coeff = (T)((i + j) % 2 ? -1 : 1);
                cofac[i][j] = minor * coeff;
            }
        }
        
        Vec<Vec<T, N>, N> adj = tp(cofac);
        return adj / d;
    }


    typedef Vec<Vec<int, 2>, 2> Mat2x2;
    typedef Vec<Vec<int, 3>, 2> Mat2x3;
    typedef Vec<Vec<int, 4>, 2> Mat2x4;
    typedef Vec<Vec<int, 2>, 3> Mat3x2;
    typedef Vec<Vec<int, 3>, 3> Mat3x3;
    typedef Vec<Vec<int, 4>, 3> Mat3x4;
    typedef Vec<Vec<int, 2>, 4> Mat4x2;
    typedef Vec<Vec<int, 3>, 4> Mat4x3;
    typedef Vec<Vec<int, 4>, 4> Mat4x4;

    typedef Vec<Vec<unsigned, 2>, 2> Mat2x2u;
    typedef Vec<Vec<unsigned, 3>, 2> Mat2x3u;
    typedef Vec<Vec<unsigned, 4>, 2> Mat2x4u;
    typedef Vec<Vec<unsigned, 2>, 3> Mat3x2u;
    typedef Vec<Vec<unsigned, 3>, 3> Mat3x3u;
    typedef Vec<Vec<unsigned, 4>, 3> Mat3x4u;
    typedef Vec<Vec<unsigned, 2>, 4> Mat4x2u;
    typedef Vec<Vec<unsigned, 3>, 4> Mat4x3u;
    typedef Vec<Vec<unsigned, 4>, 4> Mat4x4u;

    typedef Vec<Vec<float, 2>, 2> Mat2x2f;
    typedef Vec<Vec<float, 3>, 2> Mat2x3f;
    typedef Vec<Vec<float, 4>, 2> Mat2x4f;
    typedef Vec<Vec<float, 2>, 3> Mat3x2f;
    typedef Vec<Vec<float, 3>, 3> Mat3x3f;
    typedef Vec<Vec<float, 4>, 3> Mat3x4f;
    typedef Vec<Vec<float, 2>, 4> Mat4x2f;
    typedef Vec<Vec<float, 3>, 4> Mat4x3f;
    typedef Vec<Vec<float, 4>, 4> Mat4x4f;

    typedef Vec<Vec<double, 2>, 2> Mat2x2lf;
    typedef Vec<Vec<double, 3>, 2> Mat2x3lf;
    typedef Vec<Vec<double, 4>, 2> Mat2x4lf;
    typedef Vec<Vec<double, 2>, 3> Mat3x2lf;
    typedef Vec<Vec<double, 3>, 3> Mat3x3lf;
    typedef Vec<Vec<double, 4>, 3> Mat3x4lf;
    typedef Vec<Vec<double, 2>, 4> Mat4x2lf;
    typedef Vec<Vec<double, 3>, 4> Mat4x3lf;
    typedef Vec<Vec<double, 4>, 4> Mat4x4lf;

    typedef Vec<Vec<int8_t, 2>, 2> Mat2x2i8;
    typedef Vec<Vec<int8_t, 3>, 2> Mat2x3i8;
    typedef Vec<Vec<int8_t, 4>, 2> Mat2x4i8;
    typedef Vec<Vec<int8_t, 2>, 3> Mat3x2i8;
    typedef Vec<Vec<int8_t, 3>, 3> Mat3x3i8;
    typedef Vec<Vec<int8_t, 4>, 3> Mat3x4i8;
    typedef Vec<Vec<int8_t, 2>, 4> Mat4x2i8;
    typedef Vec<Vec<int8_t, 3>, 4> Mat4x3i8;
    typedef Vec<Vec<int8_t, 4>, 4> Mat4x4i8;

    typedef Vec<Vec<uint8_t, 2>, 2> Mat2x2ui8;
    typedef Vec<Vec<uint8_t, 3>, 2> Mat2x3ui8;
    typedef Vec<Vec<uint8_t, 4>, 2> Mat2x4ui8;
    typedef Vec<Vec<uint8_t, 2>, 3> Mat3x2ui8;
    typedef Vec<Vec<uint8_t, 3>, 3> Mat3x3ui8;
    typedef Vec<Vec<uint8_t, 4>, 3> Mat3x4ui8;
    typedef Vec<Vec<uint8_t, 2>, 4> Mat4x2ui8;
    typedef Vec<Vec<uint8_t, 3>, 4> Mat4x3ui8;
    typedef Vec<Vec<uint8_t, 4>, 4> Mat4x4ui8;

    typedef Vec<Vec<int16_t, 2>, 2> Mat2x2i16;
    typedef Vec<Vec<int16_t, 3>, 2> Mat2x3i16;
    typedef Vec<Vec<int16_t, 4>, 2> Mat2x4i16;
    typedef Vec<Vec<int16_t, 2>, 3> Mat3x2i16;
    typedef Vec<Vec<int16_t, 3>, 3> Mat3x3i16;
    typedef Vec<Vec<int16_t, 4>, 3> Mat3x4i16;
    typedef Vec<Vec<int16_t, 2>, 4> Mat4x2i16;
    typedef Vec<Vec<int16_t, 3>, 4> Mat4x3i16;
    typedef Vec<Vec<int16_t, 4>, 4> Mat4x4i16;

    typedef Vec<Vec<uint16_t, 2>, 2> Mat2x2ui16;
    typedef Vec<Vec<uint16_t, 3>, 2> Mat2x3ui16;
    typedef Vec<Vec<uint16_t, 4>, 2> Mat2x4ui16;
    typedef Vec<Vec<uint16_t, 2>, 3> Mat3x2ui16;
    typedef Vec<Vec<uint16_t, 3>, 3> Mat3x3ui16;
    typedef Vec<Vec<uint16_t, 4>, 3> Mat3x4ui16;
    typedef Vec<Vec<uint16_t, 2>, 4> Mat4x2ui16;
    typedef Vec<Vec<uint16_t, 3>, 4> Mat4x3ui16;
    typedef Vec<Vec<uint16_t, 4>, 4> Mat4x4ui16;

    typedef Vec<Vec<int32_t, 2>, 2> Mat2x2i32;
    typedef Vec<Vec<int32_t, 3>, 2> Mat2x3i32;
    typedef Vec<Vec<int32_t, 4>, 2> Mat2x4i32;
    typedef Vec<Vec<int32_t, 2>, 3> Mat3x2i32;
    typedef Vec<Vec<int32_t, 3>, 3> Mat3x3i32;
    typedef Vec<Vec<int32_t, 4>, 3> Mat3x4i32;
    typedef Vec<Vec<int32_t, 2>, 4> Mat4x2i32;
    typedef Vec<Vec<int32_t, 3>, 4> Mat4x3i32;
    typedef Vec<Vec<int32_t, 4>, 4> Mat4x4i32;

    typedef Vec<Vec<uint32_t, 2>, 2> Mat2x2ui32;
    typedef Vec<Vec<uint32_t, 3>, 2> Mat2x3ui32;
    typedef Vec<Vec<uint32_t, 4>, 2> Mat2x4ui32;
    typedef Vec<Vec<uint32_t, 2>, 3> Mat3x2ui32;
    typedef Vec<Vec<uint32_t, 3>, 3> Mat3x3ui32;
    typedef Vec<Vec<uint32_t, 4>, 3> Mat3x4ui32;
    typedef Vec<Vec<uint32_t, 2>, 4> Mat4x2ui32;
    typedef Vec<Vec<uint32_t, 3>, 4> Mat4x3ui32;
    typedef Vec<Vec<uint32_t, 4>, 4> Mat4x4ui32;

    typedef Vec<Vec<int64_t, 2>, 2> Mat2x2i64;
    typedef Vec<Vec<int64_t, 3>, 2> Mat2x3i64;
    typedef Vec<Vec<int64_t, 4>, 2> Mat2x4i64;
    typedef Vec<Vec<int64_t, 2>, 3> Mat3x2i64;
    typedef Vec<Vec<int64_t, 3>, 3> Mat3x3i64;
    typedef Vec<Vec<int64_t, 4>, 3> Mat3x4i64;
    typedef Vec<Vec<int64_t, 2>, 4> Mat4x2i64;
    typedef Vec<Vec<int64_t, 3>, 4> Mat4x3i64;
    typedef Vec<Vec<int64_t, 4>, 4> Mat4x4i64;

    typedef Vec<Vec<uint64_t, 2>, 2> Mat2x2ui64;
    typedef Vec<Vec<uint64_t, 3>, 2> Mat2x3ui64;
    typedef Vec<Vec<uint64_t, 4>, 2> Mat2x4ui64;
    typedef Vec<Vec<uint64_t, 2>, 3> Mat3x2ui64;
    typedef Vec<Vec<uint64_t, 3>, 3> Mat3x3ui64;
    typedef Vec<Vec<uint64_t, 4>, 3> Mat3x4ui64;
    typedef Vec<Vec<uint64_t, 2>, 4> Mat4x2ui64;
    typedef Vec<Vec<uint64_t, 3>, 4> Mat4x3ui64;
    typedef Vec<Vec<uint64_t, 4>, 4> Mat4x4ui64;

}
