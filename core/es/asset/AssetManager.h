#pragma once

#include "es/protoground.hpp"
#include "es/system/Object.hpp"
#include <memory>
#include <list>
#include <es/system/Bundle.h>

namespace es {
class IAsset;

class IAssetLoader;

class IWriter;

class AssetManager : public Object {
public:
    AssetManager() = default;

    virtual ~AssetManager() = default;

    /**
     * カスタムローダーを追加する
     *
     * 後から追加されたものが優先的に利用される。
     */
    void addLoader(const std::shared_ptr<IAssetLoader> loader);

    /**
     * カスタムローダーを削除する
     */
    void removeLoader(const std::shared_ptr<IAssetLoader> loader);

    /**
     * アセットの読み込みを行う
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path, const Bundle &hint = Bundle());

    /**
     * アセットへの書き込みを行う
     */
    virtual std::shared_ptr<IWriter> write(const std::string &path, const unsigned flags = 0, const Bundle &hint = Bundle());

private:
    std::list<std::shared_ptr<IAssetLoader> > loaders;
};

}


