#pragma once

#include <es/memory/Buffer.hpp>
#include "es/protoground.hpp"
#include "es/system/Bundle.h"

namespace es {
class IAsset;

class IWriter;

class IAssetLoader {
public:
    enum WriterFlag_e {
        /**
         * 追記を行う
         */
                WriterFlag_Append = 0x1 << 0
    };

    /**
     * アセットの読み込みを行う
     * 読み込みに失敗したらnullptrを返却する
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path, const Bundle &hint = Bundle()) = 0;

    /**
     * 永続データの書き込みを行う
     */
    virtual std::shared_ptr<IWriter> write(const std::string &path, const uint flags, const Bundle &hint = Bundle()) = 0;

    virtual ~IAssetLoader() = default;
};

}