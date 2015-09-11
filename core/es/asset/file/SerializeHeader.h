#pragma once

#include <es/memory/SafeArray.hpp>
#include "es/protoground.hpp"

namespace es {
namespace file {

struct SerializeHeader {
    enum {
        MAGIC = 0xC0DE3103,
        FORMAT_VERSION = 0x01
    };

    enum {
        /**
         * モデルデータ
         */
                DATA_UID_GRAPHICS_MODEL = 0x00010000,

        /**
         * アニメーションデータ
         */
                DATA_UID_GRAPHICS_ANIMATION,

        /**
         * 文字列テーブル
         */
                DATA_UID_SYMBOL_TABLE,
    };

    /**
     * ファイル識別子
     */
    uint32_t magic = 0xC0DE3103;

    /**
     * ファイルバージョン
     */
    uint32_t formatVersion = FORMAT_VERSION;

    /**
     * データ識別ID
     */
    uint32_t dataUid;

    /**
     * データのバイト数
     */
    uint32_t dataBytes;

    /**
     * データのサポートバージョン
     * 標準では0x01で、アップデートで古い形式サポートを行う場合はこのバージョン情報を元に設定する。
     */
    uint32_t dataVersion = 0x01;

    SerializeHeader() = default;

    SerializeHeader(uint32_t dataUid, uint32_t dataBytes) : dataUid(dataUid), dataBytes(dataBytes) { }

    /**
     * 読み出しを行う
     */
    static bool deserialize(unsafe_array<uint8_t> *buffer, SerializeHeader *result, uint32_t requestDataUid);
};

}
}
