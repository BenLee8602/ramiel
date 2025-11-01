#include <cassert>
#include "colorfmt.h"

namespace {

    uint8_t conv(float in) {
        return static_cast<uint8_t>(std::min(in, 255.0f));
    }

}

namespace ramiel {

    void cfmtRGB888(const Vec3f& in, void* out_) {
        assert(out);
        uint8_t* out = static_cast<uint8_t*>(out_);
        out[0] = conv(in[R]);
        out[1] = conv(in[G]);
        out[2] = conv(in[B]);
    }

    void cfmtBGR888(const Vec3f& in, void* out_) {
        assert(out);
        uint8_t* out = static_cast<uint8_t*>(out_);
        out[0] = conv(in[B]);
        out[1] = conv(in[G]);
        out[2] = conv(in[R]);
    }

    void cfmtRGBA8888(const Vec3f& in, void* out_) {
        assert(out);
        uint8_t* out = static_cast<uint8_t*>(out_);
        out[0] = conv(in[R]);
        out[1] = conv(in[G]);
        out[2] = conv(in[B]);
        out[3] = 255;
    }

    void cfmtBGRA8888(const Vec3f& in, void* out_) {
        assert(out);
        uint8_t* out = static_cast<uint8_t*>(out_);
        out[0] = conv(in[B]);
        out[1] = conv(in[G]);
        out[2] = conv(in[R]);
        out[3] = 255;
    }

    void cfmtARGB8888(const Vec3f& in, void* out_) {
        assert(out);
        uint8_t* out = static_cast<uint8_t*>(out_);
        out[0] = 255;
        out[1] = conv(in[R]);
        out[2] = conv(in[G]);
        out[3] = conv(in[B]);
    }

    void cfmtABGR8888(const Vec3f& in, void* out_) {
        assert(out);
        uint8_t* out = static_cast<uint8_t*>(out_);
        out[0] = 255;
        out[1] = conv(in[B]);
        out[2] = conv(in[G]);
        out[3] = conv(in[R]);
    }

}

