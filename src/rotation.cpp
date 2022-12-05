#include "rotation.h"

namespace ramiel {

    Rotation::Rotation() {
        rot = vec3f_0;
        sin = vec3f_0;
        cos = { 1.0f, 1.0f, 1.0f };
    }


    Rotation::Rotation(Vec3f rot) {
        this->rot = rot;
        sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
        cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
    }

    Rotation& Rotation::operator=(const Vec3f& rot) { set(rot); }
    Rotation& Rotation::operator+=(const Vec3f& rot) { set(this->rot + rot); }
    
    void Rotation::set(const Vec3f& rot) {
        this->rot = rot;
        sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
        cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
    }


    Rotation::operator bool()    const { return rot; }
    Rotation::operator Vec3f()   const { return rot; }
    const Vec3f& Rotation::get() const { return rot; }

    const Vec3f& Rotation::getSin() const { return sin; }
    const Vec3f& Rotation::getCos() const { return cos; }

    
    Vec3f Rotation::rotate(Vec3f in) const {
        if (!rot) return in;
        Vec3f out = in;

        // z rot
        out[X] = in[X] * cos[Z] + in[Y] * -sin[Z];
        out[Y] = in[X] * sin[Z] + in[Y] * cos[Z];
        in = out;

        // y rot
        out[X] = in[X] * cos[Y] + in[Z] * sin[Y];
        out[Z] = in[X] * -sin[Y] + in[Z] * cos[Y];
        in = out;

        // x rot
        out[Y] = in[Y] * cos[X] + in[Z] * -sin[X];
        out[Z] = in[Y] * sin[X] + in[Z] * cos[X];
        
        return out;
    }

}
