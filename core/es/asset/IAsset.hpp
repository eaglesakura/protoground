#pragma once

#include "es/protoground.hpp"
#include "es/memory/Buffer.hpp"

namespace es {
class IAsset {
public:
    /**
     * 次のサイズを読み込む
     *
     * リクエストされたsizeの容量を読み込むように努めるが、ファイル終端になった場合はsize以下の値を返却する
     */
    virtual unsafe_array<uint8_t> read(const unsigned size) = 0;

    /**
     * 残容量を取得する
     */
    virtual unsigned available() const = 0;

    virtual ~IAsset() = default;
};

namespace util {

/**
 * アセットを文字列として読み込む
 */
std::string toString(std::shared_ptr<IAsset> asset);

/**
 * byte配列に変換する
 */
ByteBuffer toByteArray(std::shared_ptr<IAsset> asset);

/**
 * 入力されたassetをメモリ上に完全コピーして、アセットを廃棄可能にする。
 */
std::shared_ptr<IAsset> toOnMemory(std::shared_ptr<IAsset> asset);

/**
 * アセットからデータを読み込む
 */
bool read(std::shared_ptr<IAsset> asset, void *ptr, const unsigned size);

}

}