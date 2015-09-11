#pragma once

#include "es/protoground.hpp"

namespace es {

class IGraphicsResource : public virtual Object {
public:
    virtual ~IGraphicsResource() = default;

    /**
     * リソースを解放する。
     *
     * デストラクタで自動的に呼び出される。
     */
    virtual void dispose() = 0;
};


}

