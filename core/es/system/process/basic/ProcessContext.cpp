#include <thread>
#include "ProcessContext.h"
#include "es/internal/protoground-internal.hpp"

#if defined(BUILD_MacOSX) || defined(BUILD_Windows)

#include "es/system/string/internal/Cpp11StringConverter.hpp"
#include "es/system/string/internal/WinStringConverter.hpp"

namespace es {

namespace {
static std::shared_ptr<ProcessContext> gInstance;
}

ProcessContext::ProcessContext() {
#if defined(BUILD_Windows)
    stringConverter.reset(new internal::WinStringConverter());
#else
    stringConverter.reset(new internal::Cpp11StringConverter());
#endif
}

ProcessContext::~ProcessContext() {

}

IProcessContext::PlatformType_e ProcessContext::getPlatform() const {
    return PlatformType_OSX;
}

void ProcessContext::onBootProcess() {
    if (gInstance) {
        return;
    }

    gInstance.reset(new ProcessContext());
}

std::shared_ptr<IProcessContext> IProcessContext::getInstance() {
    if (!gInstance) {
        eslog("Call!! ProcessContext::onBootProcess()");
        assert(false);
    }
    return gInstance;
}

void ProcessContext::newThread(const std::string &name, const IProcessContext::ThreadCallback callback) {
    string _name = name;
    std::thread newThread([_name, callback]() -> void {
#ifdef BUILD_MacOSX
        pthread_setname_np(_name.c_str());
#endif
        callback();
    });
    newThread.detach();
}

}
#endif
