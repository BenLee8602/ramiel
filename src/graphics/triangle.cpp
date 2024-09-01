#include "triangle.h"
using namespace ramiel;

namespace {

    float homogeneousCameraDepth = 0.0f;


    Vec4f intersect(
        const Vec4f& v0,
        const Vec4f& v1
    ) {
        float c = -v1[Z] / (v0[Z] - v1[Z]);
        return v1 + (v0 - v1) * c;
    }

    void clip1v(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& out
    ) {
        Vec4f vc0 = intersect(v0, v1);
        Vec4f vc2 = intersect(v2, v1);
        out.emplace_front(Tri{ v0, vc0, vc2 });
        out.emplace_front(Tri{ vc2, v2, v0 });
    }

    void clip2v(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& out
    ) {
        Vec4f vc0 = intersect(v0, v1);
        Vec4f vc2 = intersect(v2, v1);
        out.emplace_front(Tri{ vc0, v1, vc2 });
    }

}

namespace ramiel {

    TriInterpolate3d::TriInterpolate3d(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2) {
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
    void setHomogeneousCameraDepth(float z) {
        homogeneousCameraDepth = z;
    }

    bool isFrontFacing(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2) {
        Vec3f triNormal = cross(sizeView<3>(v1 - v0), sizeView<3>(v2 - v0));
        Vec3f cameraToTri = sizeView<3>(v0);
        cameraToTri[Z] -= homogeneousCameraDepth;
        return dot(triNormal, cameraToTri) < 0.0f;
    }


    bool clip(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& clippedTris
    ) {
        if (v0[Z] < 0.0f) {
            if (v1[Z] < 0.0f) {
                if (v2[Z] < 0.0f) return false;
                else clip2v(v1, v2, v0, clippedTris);
            }
            else if (v2[Z] < 0.0f) clip2v(v0, v1, v2, clippedTris);
            else clip1v(v2, v0, v1, clippedTris);
        }
        else if (v1[Z] < 0.0f) {
            if (v2[Z] < 0.0f) clip2v(v2, v0, v1, clippedTris);
            else clip1v(v0, v1, v2, clippedTris);
        }
        else if (v2[Z] < 0.0f) clip1v(v1, v2, v0, clippedTris);
        return true;
    }

}
