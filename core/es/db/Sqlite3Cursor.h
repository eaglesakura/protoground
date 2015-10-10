#pragma once

#include <es/memory/Buffer.hpp>
#include "es/protoground.hpp"

namespace es {

namespace internal {
struct SqliteCursorWrapper;
};

class Sqlite3Database;
struct SqliteEntity;

/**
 * SQLiteのrowを指し示すカーソル
 *
 * while(cursor->next()){ ... }で処理を進める必要がある。
 */
class Sqlite3Cursor : public virtual Object {
public:

    /**
     * カーソルを次へ進める。
     * 進めることが出来た場合はtrue、既に末端に到達している場合はfalseを返却する
     */
    bool next();

    /**
     * 現在の行から整数を取得する
     * NULLである場合、resultに0を格納してtrueを返却する。
     */
    bool load(const uint32_t colIndex, int64_t *result);

    /**
     * 現在の行から実数を取得する
     * NULLである場合、resultに0を格納してtrueを返却する。
     */
    bool load(const uint32_t colIndex, double *result);

    /**
     * 現在の行からテキストを取得する
     * NULLである場合、falseを返却する
     */
    bool load(const uint32_t colIndex, string *result);

    /**
     * 現在の行からBlobへのポインタをコピーする
     * メモリ自体はコピーされないため、次の行に移動したらポインタが無効になることに注意する
     */
    bool load(const uint32_t colIndex, uint32_t *resultLength, uint8_t **resultBuffer);

    /**
     * 現在の行からBlobをコピーする
     * NULLである場合、falseを返却する
     */
    bool loadWithCopy(const uint32_t colIndex, ByteBuffer *result);

    /**
     * 予め確保してあるresultバッファに対してコピーを行う。
     * もしallocatedBufferLengthが足りない場合、コピー自体を行わない。
     */
    bool loadToAllocatedBuffer(const uint32_t colIndex, uint32_t *resultLength, ByteBuffer result);

    /**
     * データ構造体に内容を読み込む
     */
    void loadToStruct(const SqliteEntity *entity, void *dataStruct);

    virtual ~Sqlite3Cursor() = default;

private:
    Sqlite3Cursor(sp<internal::SqliteCursorWrapper> impl);

    sp<internal::SqliteCursorWrapper> impl;

    friend class Sqlite3Database;
};


}


