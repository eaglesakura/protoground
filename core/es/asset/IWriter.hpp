#pragma once

#include "es/protoground.hpp"

namespace es {

/**
 * 最低限の書き込み機能を保証するWriter
 */
class IWriter {
public:
    /**
     * バッファを書き込む
     *
     * 書き込みに成功した場合trueを返却する。
     */
    virtual bool write(const void *buffer, const uint size) = 0;

    /**
     * 明示的にバッファをファイルへ反映させ、ファイルを閉じる。
     *
     * 実装しているクラスは必ずデストラクタでcloseを実行する必要がある。
     */
    virtual void close() = 0;

    virtual ~IWriter() = default;
};

}