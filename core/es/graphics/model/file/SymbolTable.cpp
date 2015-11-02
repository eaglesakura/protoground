#include <es/asset/file/SerializeHeader.h>
#include "SymbolTable.h"

namespace es {

namespace file {

namespace {
struct SymbolTable_Header {
    /**
     * シンボル数
     */
    uint32_t num;
};
}

uint64_t SymbolTable::add(const string &text) {
    return symbols.get(text).shortHash().value();
}

string SymbolTable::find(const uint64_t value) const {
    auto str = symbols.find(Hash64::restore(value));
    if (str.enable()) {
        return str.get();
    } else {
        return string("");
    }
}

ByteBuffer SymbolTable::serialize() const {

    std::vector<string> strings;
    symbols.getAllStrings(&strings);

    SymbolTable_Header header;
    header.num = strings.size();

    unsigned bufferSize = sizeof(SymbolTable_Header);
    for (const auto &str : strings) {
        // 終端NULLを含めてバッファに書き込む
        bufferSize += (str.length() + 1);
    }

    SerializeHeader serializeHeader(SerializeHeader::DATA_UID_SYMBOL_TABLE, bufferSize);

    // バッファを用意する
    ByteBuffer result = Buffer::create(bufferSize + sizeof(serializeHeader));
    uint8_t *write = result.get();
    // ヘッダを書き込む
    {
        memcpy(write, &serializeHeader, sizeof(serializeHeader));
        write += sizeof(serializeHeader);


        memcpy(write, &header, sizeof(SymbolTable_Header));
        write += sizeof(SymbolTable_Header);
    }

    // NULL込みで全データを書き込む
    for (const auto &str : strings) {
        const unsigned size = (unsigned)str.length() + 1;
        memcpy(write, str.c_str(), size);
        write += size;
    }

    return result;
}

bool SymbolTable::deserialize(unsafe_array<uint8_t> buffer) {
    SerializeHeader serializeHeader;
    if (!SerializeHeader::deserialize(&buffer, &serializeHeader, SerializeHeader::DATA_UID_SYMBOL_TABLE)) {
        return false;
    }

    uint8_t *read = buffer.ptr;

    SymbolTable_Header header;
    {
        memcpy(&header, read, sizeof(SymbolTable_Header));
        read += sizeof(SymbolTable_Header);
    }

    for (unsigned i = 0; i < header.num; ++i) {
        std::string str((char *) read);
        this->add(str);
        while (*(read++)) {
        }
    }
    return true;
}
}

}