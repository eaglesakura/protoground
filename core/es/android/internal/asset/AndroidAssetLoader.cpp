#ifdef __ANDROID__

#include    "AndroidAssetLoader.h"
#include    "AndroidMappedAsset.hpp"
#include    <android/asset_manager_jni.h>
#include "es/internal/protoground-internal.hpp"

namespace es {
namespace internal {

AndroidAssetLoader::AndroidAssetLoader(::jc::lang::object_wrapper jAssetManager, const std::string &newBasePath) :
        basePath(newBasePath) {
    this->assets = AAssetManager_fromJava(jAssetManager.getEnv(), jAssetManager.getJobject());
    assert(this->assets);
}

std::shared_ptr<IAsset> AndroidAssetLoader::load(const std::string &path, const Bundle &hint) {
    AAsset *pAsset = AAssetManager_open(assets, (basePath + path).c_str(), AASSET_MODE_BUFFER);

    if (!pAsset) {
        eslog("load error(%s)", path.c_str());
        return ::std::shared_ptr<IAsset>();
    }

    return std::shared_ptr<IAsset>(new AndroidMappedAsset(assets, pAsset));
}

std::shared_ptr<IWriter> AndroidAssetLoader::write(const std::string &path, const uint flags, const Bundle &hint) {
    // not support
    return std::shared_ptr<IWriter>();
}

}
}

#endif