#pragma once

#include "es/protoground.hpp"
#include "SafeArray.hpp"
#include "Buffer.hpp"

namespace es {

class AlignAllocator {
public:
    AlignAllocator(const uint32_t bytes) {
        buffer = Buffer::alignNew(bytes);
        pointer = unsafe_array<uint8_t>(buffer.get(), bytes);
    }

    template<typename T>
    T &push() {
        assert(pointer.length >= sizeof(T));

        uint8_t *read = pointer.ptr;
        int offset = 0;
        while (offset < align) {
            offset += sizeof(T);
        }
        pointer += offset;
        return *(static_cast<T *>(read));
    }

    ~AlignAllocator() {

    }

private:
    /**
     * メモリ境界のバイト数
     */
    uint32_t align = 16;

    /**
     * 現在指しているポインタ
     */
    unsafe_array<uint8_t> pointer;

    /**
     * 確保されたブロック
     */
    sp<uint8_t> buffer;
};
}



