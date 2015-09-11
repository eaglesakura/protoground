#ifdef __ANDROID__

#pragma once

#include "es/protoground.hpp"
#include "es/asset/IAssetLoader.hpp"
#include <jni.h>

namespace es {

class AndroidFactory {
public:
    /**
     * Assetsから読み込むためのローダーを生成する
     */
    static std::shared_ptr<IAssetLoader> newLoaderFromAssets(JNIEnv *env, jobject jContextObject, const std::string &basePath = "");

};

}

#endif