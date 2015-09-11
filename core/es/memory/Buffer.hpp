#pragma once

#include    "es/protoground.hpp"
#include    "es/memory/SafeArray.hpp"
#include    <stdlib.h>

namespace es {

class ByteBuffer {
public:
    ByteBuffer() {
        _length = 0;
    }

    ByteBuffer(sp<uint8_t> buffer, const uint _len) {
        this->buffer = buffer;
        this->_length = _len;
    }

    ByteBuffer(const ByteBuffer &cpy) {
        this->buffer = cpy.buffer;
        this->_length = cpy._length;
    }

    ~ByteBuffer() {

    }

    uint8_t *get() const {
        return buffer.get();
    }

    uint length() const {
        return _length;
    }

    bool empty() const {
        return _length == 0;
    }

    unsafe_array<uint8_t> unsafe() {
        return unsafe_array<uint8_t>(buffer.get(), _length);
    }

    std::shared_ptr<uint8_t> getSharedBuffer() const {
        return buffer;
    }

private:

    /**
     * 管理しているバッファ
     */
    std::shared_ptr<uint8_t> buffer;

    /**
     * バッファのサイズ
     */
    uint _length;
};

/**
 * メモリの配置を管理したいオブジェクトを管理する
 */
template<typename T>
class array_object {
public:
    array_object(const std::shared_ptr<uint8_t> buffer, const uint32_t offsetBytes = sizeof(T)) : buffer(buffer), offsetBytes(offsetBytes) {
        assert(buffer);
    }

    array_object(const array_object<T> &cpy) {
        this->buffer = cpy.buffer;
        this->offsetBytes = cpy.offsetBytes;
    }

    array_object() = default;

    ~array_object() = default;

    void setBuffer(const std::shared_ptr<uint8_t> &buffer, uint32_t offsetBytes = sizeof(T)) {
        this->buffer = buffer;
        this->offsetBytes = offsetBytes;
    }

    inline const T *get() const {
        assert(offsetBytes > 0);
        return (const T *) buffer.get();
    }

    inline const T *get(const int index) const {
        assert(offsetBytes > 0);
        return (const T *) (buffer.get() + (offsetBytes * index));
    }

    inline T *get(const int index) {
        assert(offsetBytes > 0);
        return (T *) (buffer.get() + (offsetBytes * index));
    }

    inline const T &operator[](const int index) const {
        return *(get(index));
    }


    inline T &operator[](const int index) {
        return *(get(index));
    }

    /**
     * unsafe_ptrでラップする
     */
    inline unsafe_array<T> unsafe(const int length) const {
        return unsafe_array<T>(get(), length);
    }

private:
    /**
     * 管理されたバッファ
     */
    std::shared_ptr<uint8_t> buffer;

    /**
     * 1オブジェクトごとのオフセット
     * 通常はsizeofで問題ないが、頂点情報のように可変長の場合は任意の設定を行う。
     */
    uint32_t offsetBytes = sizeof(T);
};


class Buffer {
    Buffer();

    ~Buffer();

    enum {
        MemoryAlignBytes = 16
    };

public:
    /**
     * バッファをコピーする
     */
    template<typename T>
    inline static ByteBuffer clone(const unsafe_array<T> &buffer) {
        ByteBuffer result = create(sizeof(T) * buffer.length);
        memcpy(result.get(), buffer.ptr, sizeof(T) * buffer.length);
        return result;
    }

    /**
     * 0クリアされたバッファを生成する
     */
    inline static ByteBuffer createZeroBuffer(uint bytes) {
        return ByteBuffer(::std::shared_ptr<uint8_t>(static_cast<uint8_t *>(calloc(1, bytes)), free), bytes);
    }

    /**
     * 指定バイト数オフセットさせたポインタを取得する
     */
    inline static void *offsetBytes(const void *origin, const int offsets) {
        return (void *) (((uint8_t *) origin) + offsets);
    }

    /**
     * 指定バイト数に境界したアドレスを返す
     */
    static uint8_t *align(uint8_t *memory) {
        // Alignmentを補正する
        memory += MemoryAlignBytes - (reinterpret_cast<uint64_t>(memory) % MemoryAlignBytes);
        assert(reinterpret_cast<uint64_t>(memory) % MemoryAlignBytes == 0);
        return memory;
    }

    /**
     * 処理が効率的になるalignに合わせてメモリを確保する
     */
    static void *alignAlloc(size_t size) {
        uint8_t *memory = (uint8_t *) ::malloc(size + MemoryAlignBytes + 1);
        // 頭部分をゼロで初期化する
        memset(memory, 0x00, MemoryAlignBytes + 1);
        memory[0] = 0xFF; // 解放すべきアドレスに目印をつける
        ++memory;
        // Alignmentを補正する
        memory += MemoryAlignBytes - (reinterpret_cast<uint64_t>(memory) % MemoryAlignBytes);
        assert(reinterpret_cast<uint64_t>(memory) % MemoryAlignBytes == 0);
        return (void *) memory;
    }

    /**
     * alignAllocしたメモリを解放する
     */
    static void alignFree(void *memory) {
        uint8_t *p = (uint8_t *) memory;
        --p;
        // 目印の値を見つけるまで頭を戻す
        while (p[0] != 0xFF) {
            --p;
        }
        ::free(p);
    }

    /**
     * Alignmentされたメモリをshared_ptrで確保する
     */
    static std::shared_ptr<uint8_t> alignNew(size_t size) {
        return std::shared_ptr<uint8_t>((uint8_t *) alignAlloc(size), Buffer::alignFree);
    }

    /**
     * バッファを確保する
     */
    inline static ByteBuffer create(uint bytes) {
        return ByteBuffer(sp<uint8_t>((uint8_t *) ::malloc(bytes), ::free), bytes);
    }

    /**
     * Alignmentを調整してバッファを生成する
     */
    inline static ByteBuffer alignCreate(uint bytes) {
        return ByteBuffer(alignNew(bytes), bytes);
    }

};

/**
 * スタックにアドレス境界された領域を生成する
 */
template<int stackSize>
class StackBuffer {
public:
    StackBuffer() {
        reset();
    }

    ~StackBuffer() = default;

    template<typename T>
    T &pop(uint offset) {
        assert((used += offset) <= stackSize);
        T *result = (T *) head;
        head += offset;
        return *result;
    }

    template<typename T>
    T &pop() {
        return pop<T>(sizeof(T));
    }

    void reset() {
        head = Buffer::align(buffer);
        assert((used = 0) == 0);
    }

private:
    uint8_t *head;
#if DEBUG
    int used;
#endif
    uint8_t buffer[stackSize + 15];
};

}
