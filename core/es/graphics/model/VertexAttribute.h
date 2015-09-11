#pragma once

#include <vector>
#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"

namespace es {

class VertexAttribute;

namespace internal {
VertexAttribute createAttribute(uint type);
};

/**
 * 属性情報は対応しているデータのみを管理する
 */
class VertexAttribute {
public:
    enum {
        MAX_BONE_WEIGHTS = 4
    };
    /**
     * 頂点の組み合わせを構築する
     */
    struct Complex {
        /**
         * 組み合わせ
         */
        uint32_t flags;

        /**
         * 頂点のバイト数
         */
        uint32_t vertexBytes;
    };

    /**
     * 位置情報
     *
     * float[3]
     */
    static const VertexAttribute POSITION_float3;

    /**
     * 位置情報.w
     *
     * float
     */
    static const VertexAttribute POSITION_W_float;

    /**
     * 法線情報
     *
     * float[3]
     */
    static const VertexAttribute NORMAL_float3;

    /**
     * 法線情報.w
     *
     * float
     */
    static const VertexAttribute NORMAL_W_float;

    /**
     * 法線情報
     * uint16_t[3] + uint16_t
     * 末尾の2byteは整列のためのダミーとして用意される。
     * 0.0〜1.0に圧縮されるため、デコードが必要になる。
     */
    static const VertexAttribute NORMAL_i4;

    /**
     * UV情報
     *
     * float[2]
     */
    static const VertexAttribute UV_float2;

    /**
     * 頂点ウェイト
     *
     * uint8_t[4]
     */
    static const VertexAttribute BONE_WEIGHT_i4;

    /**
     * 頂点インデックス
     *
     * uint16_t[4]
     */
    static const VertexAttribute BONE_INDEX_i4;

    /**
     * 頂点カラー
     *
     * uint8_t[4]
     */
    static const VertexAttribute COLOR_i4;

    /**
     * 終端ダミー
     */
    static const VertexAttribute END;

    /**
     * 容量を取得する
     */
    uint getBytes() const { return bytes; }

    /**
     * フラグ情報を取得する
     */
    uint32_t getFlag() const { return 0x1 << type; }

    /**
     * 入力されたフラグ情報のこの頂点が含まれていたらtrue
     */
    bool hasAttribute(const uint32_t flags) const { return (flags & getFlag()) != 0; }

    bool hasAttribute(const Complex &cp) const { return hasAttribute(cp.flags); }

    bool isEnd() const;

    /**
     * 属性から頂点サイズを計算する
     * 必ず終端にENDを含めなければならない。
     */
    static uint32_t getVertexBytes(const VertexAttribute *attributes);

    /**
     * 頂点の組み合わせを構築する
     * 必ず終端にENDを含めなければならない。
     */
    static Complex makeComplex(const VertexAttribute *attributes);

    /**
     * 頂点の頭から数えたバイト数を取得する
     *
     * 頂点属性が含まれていない場合、負の値を返却する。
     */
    static int32_t getVertexOffset(const VertexAttribute &attribute, const Complex &complex);

    /**
     * 書き込みヘッダを取得する
     * 頂点属性が含まれていない場合、nullptrを返却する。
     */
    static void *getWriteHeader(void *header, const VertexAttribute &attribute, const Complex &complex);

    enum Type_e {
        Type_Position_float3,
        Type_Position_W_float,
        Type_Normal_float3,
        Type_Normal_W_float,
        Type_Normal_i4,
        Type_UV_float2,
        Type_BoneWeight_i4,
        Type_BoneIndex_i4,
        Type_Color_i4,
        Type_End,
    };
private:
    /**
     * 型情報
     */
    Type_e type;

    /**
     * 容量情報
     */
    uint bytes;

    friend VertexAttribute internal::createAttribute(uint);

    VertexAttribute(Type_e type, uint32_t bytes);

};


}

