#ifdef __ANDROID__

#pragma once

#include    "es/protoground.hpp"
#include    "es/asset/IAssetLoader.hpp"
#include    "JointConnector.hpp"
#include    <jni.h>
#include    <android/asset_manager.h>


namespace es {

namespace internal {

/**
 * apkのassets内からデータを読み込む
 */
class AndroidAssetLoader : public IAssetLoader {
public:
    AndroidAssetLoader(::jc::lang::object_wrapper jAssetManager, const std::string &newBasePath = "");

    /**
     * アセットの読み込みを行う
     * 読み込みに失敗したらnullptrを返却する
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path, const Bundle &hint = Bundle()) override;

    /**
     * 永続データの書き込みを行う
     */
    virtual std::shared_ptr<IWriter> write(const std::string &path, const uint flags, const Bundle &hint = Bundle()) override;

    virtual ~AndroidAssetLoader() = default;

private:
    /**
     * Java側のAssetManagerクラス
     */
    ::jc::lang::object_wrapper jAssetManager;

    /**
     * Native Asset
     */
    AAssetManager *assets;

    std::string basePath;
};

}
}

#endif /* __ANDROID__ */