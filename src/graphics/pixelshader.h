#pragma once

#include <vector>

namespace ramiel {

    class Entity;

    class PixelShaderBase {
        friend Entity;

        virtual std::vector<uint8_t> getAttrInType() const = 0;
        virtual std::vector<uint8_t>& getAttrInPos() = 0;
    
    public:
        virtual void init(
            const float* v0,
            const float* v1,
            const float* v2
        ) = 0;

        virtual Vec3f run(const float* in) const = 0;

        virtual ~PixelShaderBase() = default;
    };

}
