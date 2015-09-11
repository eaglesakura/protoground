#pragma once

#include "es/protoground.hpp"
#include <es/memory/Buffer.hpp>
#include "es/system/string/HashStringTable.h"

namespace es {

namespace file {

class SymbolTable : public Object {
public:
    /**
     * テキストをシンボルIDに変換する
     */
    uint64_t add(const string &text);

    /**
     * シンボルテーブルから逆引きする
     * 見つからない場合、空の文字列を返す。
     */
    string find(const uint64_t value) const;

    /**
     * シリアライズを行う
     */
    ByteBuffer serialize() const;

    /**
     * シンボルテーブルを解凍する
     * 既に読み込まれている場合は古いデータを開放せず、追加を行う。
     */
    bool deserialize(unsafe_array<uint8_t> buffer);

    SymbolTable() = default;

    virtual ~SymbolTable() = default;

private:
    /**
     * 内部で管理されているシンボル
     */
    HashStringTable symbols;
};
    
}


}
