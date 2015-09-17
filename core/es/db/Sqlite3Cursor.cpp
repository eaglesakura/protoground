#include <es/db/schema/SqliteEntity.h>
#include "Sqlite3Cursor.h"
#include "internal/SqliteObjectWrapper.hpp"

namespace es {

Sqlite3Cursor::Sqlite3Cursor(sp<internal::SqliteCursorWrapper> impl) : impl(impl) {
}

bool Sqlite3Cursor::next() {
    int code = sqlite3_step(impl->cursor);
    return code == SQLITE_ROW;
}

bool Sqlite3Cursor::load(const uint32_t colIndex, int64_t *result) {
    int64_t value = sqlite3_column_int64(impl->cursor, colIndex);
    *result = value;
    return true;
}

bool Sqlite3Cursor::load(const uint32_t colIndex, double *result) {
    double value = sqlite3_column_double(impl->cursor, colIndex);
    *result = value;
    return true;
}

bool Sqlite3Cursor::load(const uint32_t colIndex, string *result) {
    const char *text = (const char *) sqlite3_column_text(impl->cursor, colIndex);
    if (!text) {
        return false;
    } else {
        (*result) = text;
        return true;
    }
}

bool Sqlite3Cursor::loadWithCopy(const uint32_t colIndex, ByteBuffer *result) {
    uint8_t *buffer = nullptr;
    uint32_t length = 0;

    if (!load(colIndex, &length, &buffer)) {
        return false;
    }
    assert(buffer);
    assert(length);

    ByteBuffer bufferWrapper = Buffer::create(length);
    memcpy(bufferWrapper.get(), buffer, length);
    *result = bufferWrapper;
    return true;
}

bool Sqlite3Cursor::loadToAllocatedBuffer(const uint32_t colIndex, uint32_t *resultLength, ByteBuffer result) {
    uint8_t *buffer = nullptr;
    uint32_t length = 0;

    if (!load(colIndex, &length, &buffer)) {
        return false;
    }

    // メモリの長さを検証する
    if (result.length() < length) {
        // メモリが足りないためコピーしない
        return false;
    }

    memcpy(result.get(), buffer, length);
    *resultLength = length;

    return true;
}

bool Sqlite3Cursor::load(const uint32_t colIndex, uint32_t *resultLength, uint8_t **resultBuffer) {
    int len = sqlite3_column_bytes(impl->cursor, colIndex);
    if (!len) {
        return false;
    }
    *resultBuffer = (uint8_t *) sqlite3_column_blob(impl->cursor, colIndex);
    *resultLength = (int32_t) len;
    return true;
}

void Sqlite3Cursor::loadToStruct(const SqliteEntity *entity, void *dataStruct) {
    assert(entity && dataStruct);

    int index = 0;
    for (const auto &prop : entity->properties) {
        const auto &bind = entity->binds[index];
        switch (prop.type) {
            case SqliteEntity::Property::Type_Int64:
                load(index, (int64_t *) Buffer::offsetBytes(dataStruct, bind.offset));
                break;
            case SqliteEntity::Property::Type_Double:
                load(index, (double *) Buffer::offsetBytes(dataStruct, bind.offset));
                break;
            case SqliteEntity::Property::Type_String:
                load(index, (string *) Buffer::offsetBytes(dataStruct, bind.offset));
                break;
            case SqliteEntity::Property::Type_Blob:
                loadWithCopy(index, (ByteBuffer *) Buffer::offsetBytes(dataStruct, bind.offset));
                break;
            default:
                assert(false);
                break;
        }
        ++index;
    }
}
}