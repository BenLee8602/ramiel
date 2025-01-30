#include "rasterize.h"
using namespace ramiel;

namespace {

    float ceil2(float n) {
        return std::ceil(n + 0.5f) - 0.5f;
    }

    float getdx(const Vec2f& v0, const Vec2f& v1) {
        return (v1[X] - v0[X]) / (v1[Y] - v0[Y]);
    }

    float getx(const Vec2f& v, float dx, float y) {
        return v[X] + dx * (y - v[Y]);
    }

}

namespace ramiel {

    void rasterizeTri(
        const Vec4f& v0_,
        const Vec4f& v1_,
        const Vec4f& v2_,
        TriPixelFn pixelFn
    ) {
        Vec2f v0 = sizeView<2>(v0_);
        Vec2f v1 = sizeView<2>(v1_);
        Vec2f v2 = sizeView<2>(v2_);

        if (v0[Y] > v1[Y]) std::swap(v0, v1);
        if (v0[Y] > v2[Y]) std::swap(v0, v2);
        if (v1[Y] > v2[Y]) std::swap(v1, v2);

        float dx0 = getdx(v0, v1);
        float dx1 = getdx(v0, v2);
        float* dx;
        if (dx0 > dx1) {
            std::swap(dx0, dx1);
            dx = &dx1;
        }
        else dx = &dx0;

        Vec2f* v = &v0;
        float y = ceil2(v0[Y]);
        float ymax = v1[Y];

        while (true) {
            for (y; y < ymax; ++y) {
                float x = ceil2(getx(*v, dx0, y));
                float xmax = getx(*v, dx1, y);

                for (x; x < xmax; ++x) {
                    pixelFn({ x, y });
                }
            }

            if (y >= v2[Y]) return;
            *dx = getdx(v1, v2);
            v = &v2;
            ymax = v2[Y];
        }
    }


    void rasterizeLine(
        const Vec4f& v0_,
        const Vec4f& v1_,
        LinePixelFn pixelFn
    ) {
        Vec2f v0 = { std::floor(v0_[X]), std::floor(v0_[Y]) };
        Vec2f v1 = { std::floor(v1_[X]), std::floor(v1_[Y]) };

        Vec2f d = v1 - v0;
        uint8_t a0 = std::abs(d[X]) > std::abs(d[Y]) ? X : Y;
        uint8_t a1 = !a0;
        d[a0] = 1.0f / d[a0];

        Vec2f inc;
        inc[a0] = v0[a0] < v1[a0] ? 1.0f : -1.0f;
        inc[a1] = inc[a0] * d[a1] * d[a0];

        for (Vec2f v = v0; v[a0] != v1[a0]; v += inc) {
            pixelFn(v, std::abs((v[a0] - v1[a0]) * d[a0]));
        }
    }

}
