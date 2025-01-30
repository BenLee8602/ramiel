#include "line.h"
#include "camera.h"
#include "clip.h"
#include "rasterize.h"
using namespace ramiel;

namespace {

    Vec4f camToScreen(Vec4f v) {
        v = getProjectionCoord(v);
        float zinv = 1.0f / v[W];
        v *= zinv;
        v = getScreenCoord(v);
        v[W] = zinv;
        v[X] = std::max(0.0f, std::floor(v[X]));
        v[Y] = std::max(0.0f, std::floor(v[Y]));
        return v;
    }

}

namespace ramiel {

    void drawLine(Vec3f v0_, Vec3f v1_, Vec3f color) {
        Vec4f v0 = { v0_[X], v0_[Y], v0_[Z], 1.0f };
        Vec4f v1 = { v1_[X], v1_[Y], v1_[Z], 1.0f };
    
        v0 = getCameraCoord(v0);
        v1 = getCameraCoord(v1);

        if (!clipLine(v0, v1)) return;

        v0 = camToScreen(v0);
        v1 = camToScreen(v1);

        rasterizeLine(v0, v1, [&](const Vec2f& p, float t) {
            Vec4f v = v0 * t + v1 * (1.0f - t);
            v[Z] /= v[W];

            size_t i = (size_t)p[X] + (size_t)p[Y] * getRes()[X];
            if (getDepthBuffer()[i] > v[Z]) {
                getDepthBuffer()[i] = v[Z];
                getColorBuffer()[i] = color;
            }
        });
    }

}
