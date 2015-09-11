#pragma once

#include    "es/protoground.hpp"
#include    <es/system/Hash64.h>
#include    <es/system/Bundle.h>

namespace es {

class IApplication : public virtual Object {
public:
    virtual ~IApplication() = default;

    virtual void onAppInitialize() = 0;

    virtual void onSurfaceChanged(int oldWidth, int oldHeight, int newWidth, int newHeight) = 0;

    virtual void onAppPause() = 0;

    virtual void onAppResume() = 0;

    virtual void onRequestRendering() = 0;

    virtual void onAppFrame() = 0;

    virtual void onAppFinalize() = 0;
protected:
    IApplication() = default;
};

}
