#include "IAsset.hpp"
#include "es/asset/internal/InMemoryAsset.hpp"
#include <vector>

namespace es {
namespace util {


/**
 * アセットを文字列として読み込む
 */
std::string toString(std::shared_ptr<IAsset> asset) {
    auto buffer = asset->read(asset->available());

    assert(!asset->available()); // 末尾まで読み込めている

    std::vector<uint8_t> temp(buffer.length + 1);
//    util::zeromemory(&temp);
    // 最後の文字だけ空白埋め
    temp[temp.size() - 1] = 0;

    memcpy(asPointer(temp), buffer.ptr, buffer.length);

    return std::string((char *) asPointer(temp));
}

/**
 * byte配列に変換する
 */
ByteBuffer toByteArray(std::shared_ptr<IAsset> asset) {
    auto buffer = asset->read(asset->available());
    ByteBuffer result = Buffer::create(buffer.length);
    memcpy(result.get(), buffer.ptr, buffer.length);
    return result;
}


std::shared_ptr<IAsset> toOnMemory(std::shared_ptr<IAsset> asset) {
    if (!asset) {
        return asset;
    }
    return std::shared_ptr<IAsset>(new internal::InMemoryAsset(asset));
}

bool read(std::shared_ptr<IAsset> asset, void *ptr, const uint size) {
    auto data = asset->read(size);
    if (data.length != size) {
        return false;
    } else {
        memcpy(ptr, data.ptr, size);
        return true;
    }
}

}
}
