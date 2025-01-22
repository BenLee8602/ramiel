#pragma once

#include "vec.h"
#include "mat.h"

namespace ramiel {

    using Quaternion = Vec4f;

    Quaternion qtn();
    Quaternion qtn(const Vec3f& axis);

    Vec3f qtnaxis(const Quaternion& q);
    Mat4x4f qtnmat(const Quaternion& q);

    Quaternion qtnqtn(const Quaternion& q1, const Quaternion& q2);
    Vec3f qtnvec(const Quaternion& q, const Vec3f& v);
    Quaternion qtnconj(const Quaternion& q);
    Quaternion qtninv(const Quaternion& q);
    Quaternion qtnslerp(const Quaternion& q1, const Quaternion& q2, float t);

}
