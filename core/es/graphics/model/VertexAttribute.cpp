#include <cmath>
#include <es/memory/Buffer.hpp>
#include "VertexAttribute.h"

namespace es {

namespace {

static const uint32_t gVertexSizeTable[] = {
        sizeof(vec3), // position
        sizeof(float), // position.w
        sizeof(vec3), // normal
        sizeof(float), // normal.w
        sizeof(uint16_t) * 4, // normal compress
        sizeof(vec2), // uv
        sizeof(uint8_t) * 4, // bone weight
        sizeof(uint16_t) * 4, // bone index
        sizeof(uint8_t) * 4, // color
        sizeof(0), // NUM
};

}

namespace internal {

VertexAttribute createAttribute(uint type) {
    assert(type <= VertexAttribute::Type_End);

    return VertexAttribute((VertexAttribute::Type_e) type, gVertexSizeTable[type]);
}

}

const VertexAttribute VertexAttribute::POSITION_float3 = internal::createAttribute(VertexAttribute::Type_Position_float3);
const VertexAttribute VertexAttribute::POSITION_W_float = internal::createAttribute(VertexAttribute::Type_Position_W_float);
const VertexAttribute VertexAttribute::NORMAL_float3 = internal::createAttribute(VertexAttribute::Type_Normal_float3);
const VertexAttribute VertexAttribute::NORMAL_W_float = internal::createAttribute(VertexAttribute::Type_Normal_W_float);
const VertexAttribute VertexAttribute::NORMAL_i4 = internal::createAttribute(VertexAttribute::Type_Normal_i4);
const VertexAttribute VertexAttribute::UV_float2 = internal::createAttribute(VertexAttribute::Type_UV_float2);
const VertexAttribute VertexAttribute::BONE_WEIGHT_i4 = internal::createAttribute(VertexAttribute::Type_BoneWeight_i4);
const VertexAttribute VertexAttribute::BONE_INDEX_i4 = internal::createAttribute(VertexAttribute::Type_BoneIndex_i4);
const VertexAttribute VertexAttribute::COLOR_i4 = internal::createAttribute(VertexAttribute::Type_Color_i4);
const VertexAttribute VertexAttribute::END = internal::createAttribute(VertexAttribute::Type_End);

VertexAttribute::VertexAttribute(Type_e type, uint32_t bytes) : type(type), bytes(bytes) {
}

uint32_t VertexAttribute::getVertexBytes(const VertexAttribute *attributes) {
    uint32_t result = 0;
    while (attributes[0].type != Type_End) {
        result += attributes[0].bytes;
        ++attributes;
    }
    return result;
}

VertexAttribute::Complex VertexAttribute::makeComplex(const VertexAttribute *attributes) {
    Complex result = {0};

    while (attributes[0].type != Type_End) {
        result.vertexBytes += attributes[0].bytes;
        result.flags |= attributes[0].getFlag();
        ++attributes;
    }
    return result;
}

int32_t VertexAttribute::getVertexOffset(const VertexAttribute &attribute, const VertexAttribute::Complex &complex) {
    if (!attribute.hasAttribute(complex.flags)) {
        return -1;
    }

    int32_t result = 0;
    for (int i = 0; i < Type_End; ++i) {
        // 含まれている頂点情報を1つずつ足し込む
        if ((complex.flags & (0x1 << i)) != 0) {
            if ((int) attribute.type == i) {
                // 自分のデータの場合はコレを返す
                return result;
            } else {
                result += gVertexSizeTable[i];
            }
        }
    }

    return -1;
}

void *VertexAttribute::getWriteHeader(void *header, const VertexAttribute &attribute, const VertexAttribute::Complex &complex) {
    const int32_t offset = getVertexOffset(attribute, complex);
    if (offset < 0) {
        return nullptr;
    } else {
        return Buffer::offsetBytes(header, offset);
    }
}

bool VertexAttribute::isEnd() const {
    return type == Type_End;
}
}