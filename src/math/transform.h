#pragma once

#include "mat.h"

namespace ramiel {

    template<typename T, size_t N>
    Vec<Vec<T, N + 1>, N + 1> translate(const Vec<T, N>& translation) {
        Vec<Vec<T, N + 1>, N + 1> mat = id<T, N + 1>();
        for (size_t i = 0; i < N; ++i)
            mat[i][N] = translation[i];
        return mat;
    }

    template<typename T>
    Vec<Vec<T, 3>, 3> rotate(const T& rad) {
        T sino = std::sin(rad);
        T coso = std::cos(rad);

        return Vec<Vec<T, 3>, 3> {
            Vec<T, 3>{  coso, -sino,   0   },
            Vec<T, 3>{  sino,  coso,   0   },
            Vec<T, 3>{   0,     0,     1   }
        };
    }


    template<typename T>
    Vec<Vec<T, 4>, 4> rotate(const Vec<T, 3>& rad) {
        Vec<T, 3> sino = {
            std::sin(rad[X]),
            std::sin(rad[Y]),
            std::sin(rad[Z])
        };
        Vec<T, 3> coso = {
            std::cos(rad[X]),
            std::cos(rad[Y]),
            std::cos(rad[Z])
        };

        return Vec<Vec<T, 4>, 4> { Vec<T, 4> {
            coso[Y] * coso[Z],
            -coso[Y] * sino[Z],
            sino[Y],
            0
        }, Vec<T, 4> {
            sino[X] * sino[Y] * coso[Z] + coso[X] * sino[Z],
            coso[X] * coso[Z] - sino[X] * sino[Y] * sino[Z],
            -sino[X] * coso[Y],
            0
        }, Vec<T, 4> {
            sino[X] * sino[Z] - coso[X] * sino[Y] * coso[Z],
            coso[X] * sino[Y] * sino[Z] + sino[X] * coso[Z],
            coso[X] * coso[Y],
            0
        }, Vec<T, 4> { 0, 0, 0, 1 } };
    }


    template<typename T, size_t N>
    Vec<Vec<T, N + 1>, N + 1> scale(const Vec<T, N>& factor) {
        Vec<Vec<T, N + 1>, N + 1> mat = id<T, N + 1>();
        for (size_t i = 0; i < N; ++i)
            mat[i][i] = factor[i];
        return mat;
    }

}
