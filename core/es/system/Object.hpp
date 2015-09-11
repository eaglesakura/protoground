#pragma once

#include    <cstdint>

namespace es {

enum InterfaceId_e {
    InterfaceId_OpenGL_Engine = 0x01000000,
    InterfaceId_OpenGL_GPUCapacity,
    InterfaceId_OpenGL_Device,
    InterfaceId_OpenGL_Surface,
    InterfaceId_OpenGL_Texture,
    InterfaceId_OpenGL_Mesh,
};

/**
 * 継承関係が複雑になる場合に使用するRoot Object
 *
 * vtableが生成されるため、オブジェクトサイズが増えることに注意する。
 */
class Object {
public:
    enum QueryResult_e {
        QueryResult_Failed = 0,
        QueryResult_Success = 1,
    };
    typedef std::shared_ptr<Object> shared;

    Object() = default;

    virtual ~Object() = default;

    /**
     * dynamic_castのオーバーヘッドを避けたい場合にインターフェース変換を行わせることが出来る。
     *
     * ただし、型安全ではないため、利用箇所には注意をすること。
     */
    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
        return QueryResult_Failed;
    }

protected:
    template<typename T>
    static QueryResult_e query_cast(const T *self, void **resultInterfacePtr) {
        *((T **) resultInterfacePtr) = (T *) self;
        return QueryResult_Success;
    }

private:
    /**
     * 明示的にコピーを禁止する
     */
    void operator=(const Object &cpy) = delete;

    /**
     * 明示的にコピーを禁止する
     */
    Object(const Object &cpy) = delete;
};


/**
 * Objectの変換を行う。
 * typo防止のため、マクロとして扱う。
 */
#define ES_SUPPORT_QUERY(id, type)  if(interfaceId == id){ return Object::query_cast<type>(static_cast<const type *>(this), resultInterfacePtr); }


}
