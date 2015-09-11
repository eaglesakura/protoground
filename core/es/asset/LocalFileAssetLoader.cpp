#include "LocalFileAssetLoader.h"
#include "es/asset/internal/InMemoryAsset.hpp"
#include "es/asset/internal/LocalFileWriter.hpp"

namespace es {


LocalFileAssetLoader::LocalFileAssetLoader(const std::string &_basePath, const uint _flags) : basePath(_basePath), flags(_flags) {
}

std::shared_ptr<IAsset> LocalFileAssetLoader::load(const std::string &path, const Bundle &hint) {
    if (!(flags & Flag_Read)) {
        // 読み込みをサポートしていない
        return sp<IAsset>();
    }

    std::ifstream stream(basePath + path, std::ifstream::in | std::ifstream::binary);
    if (stream.fail()) {
//        eslog("LoadFiled(%s)", (basePath + path).c_str());
        return std::shared_ptr<IAsset>();
    }

    return std::shared_ptr<IAsset>(new internal::InMemoryAsset(stream));
}

std::shared_ptr<IWriter> LocalFileAssetLoader::write(const std::string &path, const uint inFlag, const Bundle &hint) {
    if (!(this->flags & Flag_Write)) {
        // 書き込みをサポートしていない
        return sp<IWriter>();
    }

    sp<internal::LocalFileWriter> result(new internal::LocalFileWriter());
    if (!result->open(basePath + path, (inFlag & WriterFlag_Append) != 0)) {
        return sp<IWriter>();
    } else {
        return result;
    }
}

}