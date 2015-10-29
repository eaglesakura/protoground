#pragma once

#include "IAsset.hpp"
#include "IAssetLoader.hpp"

namespace es {

/**
 * ローカルファイル(SDカードやSSD)を読み書きする
 */
class LocalFileAssetLoader : public Object, public IAssetLoader {
public:
    enum {
        /**
         * 読み込みに対応する
         */
                Flag_Read = 0x1 << 0,

        /**
         * 書き込みに対応する
         */
                Flag_Write = 0x1 << 1,
    };

    /**
     *
     */
    LocalFileAssetLoader(const std::string &_basePath = "", const unsigned flags = Flag_Read);

    virtual ~LocalFileAssetLoader() = default;

    /**
     * アセットの読み込みを行う
     * 読み込みに失敗したらnullptrを返却する
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path, const Bundle &hint) override ;


    virtual std::shared_ptr<IWriter> write(const std::string &path, const unsigned flags, const Bundle &hint) override;

private:
    uint32_t flags = 0;
    std::string basePath;
};

}


