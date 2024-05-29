#pragma once

#include <typeindex>
#include <functional>

namespace ramiel {

    class VoidBuffer {
    public:
        typedef std::function<void*(size_t)> Allocator;
        typedef std::function<void(void*)> Deallocator;

        VoidBuffer(
            std::type_index type,
            size_t size,
            Allocator allocate,
            Deallocator deallocate,
            size_t length = 0
        );
        VoidBuffer(const VoidBuffer& other) = delete;
        VoidBuffer(VoidBuffer&& other) = delete;
        VoidBuffer& operator=(const VoidBuffer& other) = delete;
        VoidBuffer& operator=(VoidBuffer&& other) = delete;
        ~VoidBuffer();

        std::type_index getType() const;
        size_t getSize() const;
        size_t getLength() const;

        void setLength(size_t length);

        void* operator[](size_t i);
        const void* operator[](size_t i) const;

    private:
        const std::type_index type;
        const size_t size;

        void* data;
        size_t length;

        const Allocator allocate;
        const Deallocator deallocate;
    };


    template<class T>
    VoidBuffer makeVoidBuffer(size_t length = 0) {
        VoidBuffer::Allocator allocate = [](size_t length) {
            return static_cast<void*>(new T[length]());
        };

        VoidBuffer::Deallocator deallocate = [](void* data) {
            delete[] static_cast<T*>(data);
        };

        return VoidBuffer(typeid(T), sizeof(T), allocate, deallocate, length);
    }

}
