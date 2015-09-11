#ifdef __ANDROID__

#include "AndroidFactory.h"
#include "JointConnector.hpp"
#include "es/android/internal/asset/AndroidAssetLoader.h"

namespace es {

std::shared_ptr<IAssetLoader> AndroidFactory::newLoaderFromAssets(JNIEnv *env, jobject jContextObject, const std::string &basePath) {
    ::jc::lang::object_wrapper context(jContextObject, env, false);

    std::shared_ptr<IAssetLoader> result(new internal::AndroidAssetLoader(jContextObject, basePath));
    return result;

}

}

#endif