#pragma once

#include <vector>

namespace ramiel {

    class Entity;

    class VertexShaderBase {
        friend Entity;

        virtual std::vector<uint8_t> getAttrInType() const = 0;
        virtual std::vector<uint8_t>& getAttrInPos() = 0;

        virtual std::vector<uint8_t> getAttrOutType() const = 0;
        virtual std::vector<uint8_t> getAttrOutPos() const = 0;
        virtual size_t getAttrOutSize() const = 0;
    
    public:
        virtual void run(const float* in, float* out) const = 0;

        virtual ~VertexShaderBase() = default;
    };

}
