#include "triangle.h"
using namespace ramiel;

namespace {

    struct Plane {
        std::string name;
        Vec3f pos;
        Vec3f nml;
    };
    
    std::array<Plane, 6> viewFrustum;

    bool comparePlanePoint(
        const Plane& p,
        const Vec4f& v
    ) {
        return dot(p.nml, p.pos - sizeView<3>(v)) < 0.0f;
    }

    Vec4f intersectPlaneLine(
        const Plane& p,
        const Vec4f& l0,
        const Vec4f& l1
    ) {
        const Vec3f& l0_3 = sizeView<3>(l0);
        const Vec3f& l1_3 = sizeView<3>(l1);
        float d = dot(p.pos - l0_3, p.nml) / dot(l1_3, p.nml);
        return l0 + l1 * d;
    }


    uint8_t clip1v(
        const Plane& p,
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& out
    ) {
        Vec4f vc0 = intersectPlaneLine(p, v1, v0 - v1);
        Vec4f vc2 = intersectPlaneLine(p, v1, v2 - v1);
        out.emplace_front(Tri{ v0, vc0, vc2 });
        out.emplace_front(Tri{ vc2, v2, v0 });
        return 1;
    }

    uint8_t clip2v(
        const Plane& p,
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& out
    ) {
        Vec4f vc0 = intersectPlaneLine(p, v1, v0 - v1);
        Vec4f vc2 = intersectPlaneLine(p, v1, v2 - v1);
        out.emplace_front(Tri{ vc0, v1, vc2 });
        return 2;
    }


    uint8_t clipTri(
        const Plane& p,
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& clippedTris
    ) {
        if (comparePlanePoint(p, v0)) {
            if (comparePlanePoint(p, v1)) {
                if (comparePlanePoint(p, v2)) return 3;
                return clip2v(p, v1, v2, v0, clippedTris);
            }
            if (comparePlanePoint(p, v2)) return clip2v(p, v0, v1, v2, clippedTris);
            return clip1v(p, v2, v0, v1, clippedTris);
        }
        if (comparePlanePoint(p, v1)) {
            if (comparePlanePoint(p, v2)) return clip2v(p, v2, v0, v1, clippedTris);
            return clip1v(p, v0, v1, v2, clippedTris);
        }
        if (comparePlanePoint(p, v2)) return clip1v(p, v1, v2, v0, clippedTris);
        return 0;
    }


    bool clipEachTri(const Plane& p, TriList& tris) {
        TriList clippedTris;
        tris.remove_if([&p, &clippedTris](const Tri& t) {
            return clipTri(p, t[0], t[1], t[2], clippedTris) != 0;
        });
        tris.splice(tris.end(), clippedTris);
        return !tris.empty();
    }


    bool isFrontFacing(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2) {
        Vec3f triNormal = cross(sizeView<3>(v1 - v0), sizeView<3>(v2 - v0));
        return dot(triNormal, sizeView<3>(v0)) < 0.0f;
    }

}

namespace ramiel {

    TriInterpolate3d::TriInterpolate3d(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2
    ) {
        this->v1 = sizeView<3>(v1);
        x = sizeView<3>(v0 - v1);
        y = sizeView<3>(v2 - v1);
        z = normalize(cross(x, y));
        a = 1.0f / det(Mat3x3f{ x, y, z });
    }

    Vec3f TriInterpolate3d::operator()(const Vec4f& p) const {
        Vec3f vp = sizeView<3>(p) - v1;
        Vec3f weights;
        weights[0] = det(Mat3x3f{ vp, y, z }) * a;
        weights[2] = det(Mat3x3f{ x, vp, z }) * a;
        weights[1] = 1.0f - (weights[0] + weights[2]);
        return weights;
    }


    // declaration in camera.cpp ;)
    void updateViewFrustum() {
        float fx = -1.0f / getFocalLen();
        float fy = -1.0f / (getFocalLen() * getAspectRatio());

        viewFrustum = {
            Plane{
                "-x",
                {  0.0f,  0.0f, 0.0f },
                { -1.0f,  0.0f, fx   }
            }, {
                "+x",
                {  0.0f,  0.0f, 0.0f },
                {  1.0f,  0.0f, fx   }
            }, {
                "-y",
                {  0.0f,  0.0f, 0.0f },
                {  0.0f, -1.0f, fy   }
            }, {
                "+y",
                {  0.0f,  0.0f, 0.0f },
                {  0.0f,  1.0f, fy   }
            }, {
                "-z",
                {  0.0f,  0.0f, getZ0() },
                {  0.0f,  0.0f, -1.0f   }
            }, {
                "+z",
                {  0.0f,  0.0f, getZ1() },
                {  0.0f,  0.0f,  1.0f   }
            }
        };
    }


    bool clip(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& clippedTris
    ) {
        if (!isFrontFacing(v0, v1, v2)) return false;
        for (auto& p : viewFrustum) {
            if (clippedTris.empty()) {
                if (clipTri(p, v0, v1, v2, clippedTris) == 3) return false;
            }
            else if (!clipEachTri(p, clippedTris)) return false;
        }
        return true;
    }

}
