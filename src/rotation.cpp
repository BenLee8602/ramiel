#include "rotation.h"

namespace ramiel {

    Rotation::Rotation() {
        rot = vec3f_0;
        sine = vec3f_0;
        cosine = { 1.0f, 1.0f, 1.0f };
    }


    Rotation::Rotation(Vec3f rot) {
        this->rot = rot;
        sine = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
        cosine = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
    }

    Rotation& Rotation::operator=(const Vec3f& rot)  { set(rot); return *this; }
    Rotation& Rotation::operator+=(const Vec3f& rot) { set(this->rot + rot); return *this; }
    
    void Rotation::set(const Vec3f& rot) {
        this->rot = rot;
        sine = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
        cosine = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
    }


    float Rotation::operator[](size_t i) const { return rot[i]; }
    Rotation::operator bool()    const { return rot; }
    Rotation::operator Vec3f()   const { return rot; }
    const Vec3f& Rotation::get() const { return rot; }

    const Vec3f& Rotation::sin() const { return sine; }
    const Vec3f& Rotation::cos() const { return cosine; }

    
    Vec3f Rotation::rotate(Vec3f in) const {
        if (!rot) return in;
        Vec3f out = in;

        // z rot
        out[X] = in[X] * cosine[Z] + in[Y] * -sine[Z];
        out[Y] = in[X] * sine[Z]   + in[Y] *  cosine[Z];
        in = out;

        // y rot
        out[X] = in[X] *  cosine[Y] + in[Z] * sine[Y];
        out[Z] = in[X] * -sine[Y]   + in[Z] * cosine[Y];
        in = out;

        // x rot
        out[Y] = in[Y] * cosine[X] + in[Z] * -sine[X];
        out[Z] = in[Y] * sine[X]   + in[Z] *  cosine[X];
        
        return out;
    }

}
