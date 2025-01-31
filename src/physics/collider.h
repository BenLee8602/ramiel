#pragma once

#include <typeindex>

namespace ramiel {

    class Collider {
    public:
        using Type = std::type_index;
        virtual Type getColliderType() const = 0;
    };

}
