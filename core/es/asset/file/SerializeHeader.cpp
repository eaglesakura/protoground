#include "SerializeHeader.h"

namespace es {
namespace file {

bool SerializeHeader::deserialize(unsafe_array<uint8_t> *buffer, SerializeHeader *result, uint32_t requestDataUid) {
    assert(buffer && buffer->ptr);
    assert(result);

    if (buffer->length < sizeof(SerializeHeader)) {
        return false;
    }

    memcpy(result, buffer->ptr, sizeof(SerializeHeader));
    (*buffer) += sizeof(SerializeHeader);

    // マジックナンバーをチェック
    if (result->magic != MAGIC) {
        return false;
    }
    if (result->formatVersion > FORMAT_VERSION) {
        return false;
    }
    if (result->dataBytes > buffer->length) {
        // データ長も足りない
        return false;
    }
    if (result->dataUid != requestDataUid) {
        // データUIDが違う
        return false;
    }

    return true;
}
}
}