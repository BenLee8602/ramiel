#pragma once

#include <typeindex>
#include <functional>

namespace ramiel {

    class VoidBuffer {
    public:
        typedef std::function<void*(size_t)> Allocator;
        typedef std::function<void(void*)> Deallocator;
        typedef std::function<void(void*, const void*)> Copier;

        VoidBuffer(
            std::type_index type,
            size_t size,
            Allocator allocate,
            Deallocator deallocate,
            Copier copier = nullptr,
            size_t length = 0
        );
        VoidBuffer(const VoidBuffer& src);
        VoidBuffer(VoidBuffer&& src);
        VoidBuffer& operator=(const VoidBuffer& src);
        VoidBuffer& operator=(VoidBuffer&& src);
        ~VoidBuffer();

        std::type_index getType() const;
        size_t getSize() const;
        size_t getLength() const;

        void setLength(size_t length);

        void* operator[](size_t i);
        const void* operator[](size_t i) const;

    private:
        std::type_index type;
        size_t size;

        Allocator allocate;
        Deallocator deallocate;
        Copier copy;

        void* data;
        size_t length;
    };


    template<class T>
    VoidBuffer makeVoidBuffer(size_t length = 0) {
        VoidBuffer::Allocator allocate = [](size_t length) {
            return static_cast<void*>(new T[length]());
        };

        VoidBuffer::Deallocator deallocate = [](void* data) {
            delete[] static_cast<T*>(data);
        };

        VoidBuffer::Copier copy = [](void* dest, const void* src) {
            *static_cast<T*>(dest) = *static_cast<const T*>(src);
        };

        return VoidBuffer(typeid(T), sizeof(T), allocate, deallocate, copy, length);
    }

}
