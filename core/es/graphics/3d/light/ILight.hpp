#pragma once

#include <es/protoground.hpp>

namespace es {

/**
 * ライト
 */
class ILight : public virtual Object {
public:
    /**
     * シャドウ対応のライトであればtrueを
     */
    virtual bool isShadowMapping() const = 0;

    virtual ~ILight() = default;
};

/**
 * シャドウ対応ライトであればtrue
 */
class IShadowLight : public virtual ILight {
public:
    virtual bool isShadowMapping() const override {
        return true;
    }
};

}