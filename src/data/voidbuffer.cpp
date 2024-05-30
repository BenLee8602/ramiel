#include <cassert>
#include "voidbuffer.h"

namespace ramiel {

    VoidBuffer::VoidBuffer(
        std::type_index type,
        size_t size,
        Allocator allocate,
        Deallocator deallocate,
        Copier copy,
        size_t length
    ) : type(type)
      , size(size)
      , allocate(allocate)
      , deallocate(deallocate)
      , copy(copy)
      , length(length)
    {
        assert(size       && "VoidBuffer::VoidBuffer - size must not be 0");
        assert(allocate   && "VoidBuffer::VoidBuffer - must provide an allocator");
        assert(deallocate && "VoidBuffer::VoidBuffer - must provide a deallocator");
        data = length ? allocate(length) : nullptr;
    }


    VoidBuffer::VoidBuffer(const VoidBuffer& src) : type(src.type) {
        assert(src.copy && "VoidBuffer::VoidBuffer(VoidBuffer) - source must be copyable");

        size = src.size;
        allocate = src.allocate;
        deallocate = src.deallocate;
        copy = src.copy;
        length = src.length;

        data = length ? allocate(length) : nullptr;
        char* d = static_cast<char*>(data);
        const char* s = static_cast<const char*>(src.data);
        for (size_t i = 0; i < length; ++i) {
            copy(d, s);
            d += size;
            s += size;
        }
    }


    VoidBuffer::VoidBuffer(VoidBuffer&& src) : type(src.type) {
        size = src.size;
        allocate = src.allocate;
        deallocate = src.deallocate;
        copy = src.copy;
        data = src.data;
        length = src.length;

        src.data = nullptr;
        src.length = 0;
    }
    

    VoidBuffer& VoidBuffer::operator=(const VoidBuffer& src) {
        assert(src.copy && "VoidBuffer::operator=(VoidBuffer) - source must be copyable");

        if (data) deallocate(data);

        type = src.type;
        size = src.size;
        allocate = src.allocate;
        deallocate = src.deallocate;
        copy = src.copy;
        length = src.length;

        data = length ? allocate(length) : nullptr;
        char* d = static_cast<char*>(data);
        const char* s = static_cast<const char*>(src.data);
        for (size_t i = 0; i < length; ++i) {
            copy(d, s);
            d += size;
            s += size;
        }

        return *this;
    }
    

    VoidBuffer& VoidBuffer::operator=(VoidBuffer&& src) {
        if (data) deallocate(data);

        type = src.type;
        size = src.size;
        allocate = src.allocate;
        deallocate = src.deallocate;
        copy = src.copy;
        data = src.data;
        length = src.length;

        src.data = nullptr;
        src.length = 0;

        return *this;
    }


    VoidBuffer::~VoidBuffer() {
        if (data) deallocate(data);
    }


    std::type_index VoidBuffer::getType() const {
        return type;
    }

    size_t VoidBuffer::getSize() const {
        return size;
    }
    
    size_t VoidBuffer::getLength() const {
        return length;
    }


    void VoidBuffer::setLength(size_t length) {
        if (data) deallocate(data);
        data = length ? allocate(length) : nullptr;
        this->length = length;
    }
    

    void* VoidBuffer::operator[](size_t i) {
        assert(data && "VoidBuffer::operator[] - cannot access empty buffer");
        assert(i < length && "VoidBuffer::operator[] - index out of range");
        return static_cast<char*>(data) + size * i;
    }
    
    const void* VoidBuffer::operator[](size_t i) const {
        assert(data && "VoidBuffer::operator[] - cannot access empty buffer");
        assert(i < length && "VoidBuffer::operator[] - index out of range");
        return static_cast<const char*>(data) + size * i;
    }

}
