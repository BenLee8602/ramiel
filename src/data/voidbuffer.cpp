#include <cassert>
#include "voidbuffer.h"

namespace ramiel {

    VoidBuffer::VoidBuffer(
        std::type_index type,
        size_t size,
        Allocator allocate,
        Deallocator deallocate,
        size_t length
    ) : type(type)
      , size(size)
      , length(length)
      , allocate(allocate)
      , deallocate(deallocate)
    {
        assert(size       && "VoidBuffer::VoidBuffer - size must not be 0");
        assert(allocate   && "VoidBuffer::VoidBuffer - must provide an allocator");
        assert(deallocate && "VoidBuffer::VoidBuffer - must provide a deallocator");
        data = length ? allocate(length) : nullptr;
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
