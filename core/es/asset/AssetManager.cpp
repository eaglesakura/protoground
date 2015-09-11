#include "AssetManager.h"
#include "es/internal/protoground-internal.hpp"

#include "es/asset/IAsset.hpp"
#include "es/asset/IAssetLoader.hpp"
#include <list>

namespace es {

void AssetManager::addLoader(const std::shared_ptr<IAssetLoader> loader) {
    loaders.push_front(loader);
}

void AssetManager::removeLoader(const std::shared_ptr<IAssetLoader> loader) {
    loaders.remove(loader);
}

std::shared_ptr<IAsset> AssetManager::load(const std::string &path, const Bundle &hint) {

    for (std::shared_ptr<IAssetLoader> &loader : loaders) {
        auto result = loader->load(path, hint);
        if (result) {
            return result;
        }
    }

    eslog("LoadFiled(%s)", path.c_str());
    return std::shared_ptr<IAsset>();
}

std::shared_ptr<IWriter> AssetManager::write(const std::string &path, const uint flags, const Bundle &hint) {
    for (std::shared_ptr<IAssetLoader> &loader : loaders) {
        auto result = loader->write(path, flags, hint);
        if (result) {
            return result;
        }
    }

    eslog("WriteFailed(%s)", path.c_str());
    return std::shared_ptr<IWriter>();
}
}
