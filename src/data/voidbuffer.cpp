#include <cassert>
#include "voidbuffer.h"

namespace ramiel {

    VoidBuffer::VoidBuffer(
        std::type_index type,
        size_t size,
        size_t length,
        Allocator allocate,
        Deallocator deallocate
    ) : type(type)
      , size(size)
      , length(length)
      , allocate(allocate)
      , deallocate(deallocate)
    {
        assert(size       && "VoidBuffer::VoidBuffer - size must not be 0");
        assert(length     && "VoidBuffer::VoidBuffer - length must not be 0");
        assert(allocate   && "VoidBuffer::VoidBuffer - must provide an allocator");
        assert(deallocate && "VoidBuffer::VoidBuffer - must provide a deallocator");
        data = allocate();
    }

    VoidBuffer::~VoidBuffer() {
        deallocate(data);
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
    

    void* VoidBuffer::operator[](size_t i) {
        assert(i < length && "VoidBuffer::operator[] - index out of range");
        return static_cast<char*>(data) + size * i;
    }
    
    const void* VoidBuffer::operator[](size_t i) const {
        assert(i < length && "VoidBuffer::operator[] - index out of range");
        return static_cast<const char*>(data) + size * i;
    }

}
