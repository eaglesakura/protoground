#ifdef __ANDROID__
#pragma once

#include "es/protoground.hpp"
#include "es/asset/IAsset.hpp"
#include "android/asset_manager.h"

namespace es {
namespace internal {

/**
 * メモリマッピングしたアセットを管理する
 */
class AndroidMappedAsset : public IAsset {
    AAsset *pAsset;

    AAssetManager *pAssetManager;

    /**
     * 残りのバッファ
     */
    unsafe_array<uint8_t> buffer;
public:
    AndroidMappedAsset(AAssetManager *pAssetManager, AAsset *pAsset) {
        this->pAssetManager = pAssetManager;
        this->pAsset = pAsset;
        this->buffer.ptr = (uint8_t *) AAsset_getBuffer(pAsset);
        this->buffer.length = AAsset_getLength(pAsset);

        assert(pAssetManager);
        assert(pAsset);
        assert((bool) buffer);
    }

    virtual ~AndroidMappedAsset() {
        AAsset_close(pAsset);
    }

    /**
     * 次のサイズを読み込む
     *
     * リクエストされたsizeの容量を読み込むように努めるが、ファイル終端になった場合はsize以下の値を返却する
     */
    virtual unsafe_array<uint8_t> read(const unsigned size) {
        if (!available()) {
            // 容量が無ければ何もしない
            return unsafe_array<uint8_t>();
        }

        // ポインタをコピーする
        unsafe_array<uint8_t> result = buffer;
        result.length = std::min(result.length, (int) size);

        buffer += result.length;

        return result;
    }

    /**
     * 残容量を取得する
     */
    virtual unsigned available() const {
        return buffer.length;
    }
};


}
}

#endif