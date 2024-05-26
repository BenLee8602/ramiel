#pragma once

#include <typeindex>
#include <functional>

namespace ramiel {

    class VoidBuffer {
    public:
        typedef std::function<void*(void)> Allocator;
        typedef std::function<void(void*)> Deallocator;

        VoidBuffer(
            std::type_index type,
            size_t size,
            size_t length,
            Allocator allocate,
            Deallocator deallocate
        );
        VoidBuffer(const VoidBuffer& other) = delete;
        VoidBuffer(VoidBuffer&& other) = delete;
        VoidBuffer& operator=(const VoidBuffer& other) = delete;
        VoidBuffer& operator=(VoidBuffer&& other) = delete;
        ~VoidBuffer();

        std::type_index getType() const;
        size_t getSize() const;
        size_t getLength() const;

        void* operator[](size_t i);
        const void* operator[](size_t i) const;

    private:
        const std::type_index type;
        const size_t size;

        void* data;
        const size_t length;

        const Allocator allocate;
        const Deallocator deallocate;
    };


    template<class T>
    VoidBuffer makeVoidBuffer(size_t length) {
        VoidBuffer::Allocator allocate = [length]() {
            return static_cast<void*>(new T[length]());
        };

        VoidBuffer::Deallocator deallocate = [](void* data) {
            delete[] static_cast<T*>(data);
        };

        return VoidBuffer(typeid(T), sizeof(T), length, allocate, deallocate);
    }

}
