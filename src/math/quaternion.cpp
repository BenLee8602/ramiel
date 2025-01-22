#include <cassert>
#include "quaternion.h"

namespace ramiel {

    Quaternion qtn() {
        return { 0, 0, 0, 1 };
    }


    Quaternion qtn(const Vec3f& axis) {
        float theta = mag(axis);
        float w = cos(theta / 2);
        Vec3f v = normalize(axis, theta) * sin(theta / 2);
        return { v[X], v[Y], v[Z], w };
    }


    Vec3f qtnaxis(const Quaternion& q) {
        assert(abs(mag(q) - 1.0f) < 1e-6f);
        return normalize(sizeView<3>(q)) * 2 * acos(q[W]);
    }


    Mat4x4f qtnmat(const Quaternion& q) {
        assert(abs(mag(q) - 1.0f) < 1e-6f);

        const float& w = q[W];
        const float& x = q[X];
        const float& y = q[Y];
        const float& z = q[Z];

        return {
            Vec4f{ 1 - 2*y*y - 2*z*z,     2*x*y - 2*z*w,     2*x*z + 2*y*w, 0 },
            Vec4f{     2*x*y + 2*z*w, 1 - 2*x*x - 2*z*z,     2*y*z - 2*x*w, 0 },
            Vec4f{     2*x*z - 2*y*w,     2*y*z + 2*x*w, 1 - 2*x*x - 2*y*y, 0 },
            Vec4f{           0,                 0,                 0,       1 }
        };
    }


    Quaternion qtnqtn(const Quaternion& q1, const Quaternion& q2) {
        Vec4f q;

        auto& v1 = sizeView<3>(q1);
        auto& v2 = sizeView<3>(q2);
        auto& v = sizeView<3>(q);

        auto& w1 = q1[W];
        auto& w2 = q2[W];
        auto& w = q[W];

        v = v2 * w1 + v1 * w2 + cross(v1, v2);
        w = w1 * w2 - dot(v1, v2);
        return q;
    }


    Vec3f qtnvec(const Quaternion& q, const Vec3f& v) {
        assert(abs(mag(q) - 1.0f) < 1e-6f);
        Quaternion v_ = { v[X], v[Y], v[Z], 0.0f };
        v_ = qtnqtn(q, v_);
        v_ = qtnqtn(v_, qtnconj(q));
        return { v_[X], v_[Y], v_[Z] };
    }


    Quaternion qtnconj(const Quaternion& q) {
        return { -q[X], -q[Y], -q[Z], q[W] };
    }


    Quaternion qtninv(const Quaternion& q) {
        return qtnconj(q) / dot(q, q);
    }


    Quaternion qtnslerp(const Quaternion& q1, const Quaternion& q2, float t) {
        float theta = acos(dot(q1, q2));
        return (q1 * sin((1.0f - t) * theta) + q2 * sin(t * theta)) / sin(theta);
    }

}
