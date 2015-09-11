#pragma once

#include "es/protoground.hpp"
#include "es/graphics/model/VertexAttribute.h"

namespace es {

class ISkinningController {
public:
    enum Flag_e {
        Flag_ComputeMorph = 0x1 << 0,
    };

    /**
     * スキニングを実行する
     *
     */
    virtual void compute(
            const uint32_t flags,
            const VertexAttribute::Complex &dstComplex, void *__restrict dstVertices
    ) const = 0;
};

}