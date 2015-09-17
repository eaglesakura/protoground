#pragma once

#include "es/protoground.hpp"
#include "Sqlite3Cursor.h"
#include "schema/SqliteSchema.h"
#include "schema/SqliteEntity.h"

namespace es {

namespace internal {
struct SqliteDbWrapper;
}

/**
 * sqlite3 DB管理のラッパークラス
 */
class Sqlite3Database : public virtual Object {
public:
    enum OpenFlag_e {
        OpenFlag_ReadOnly,
        OpenFlag_ReadWrite,
    };

    enum Result_e {
        Result_OK = 0,
        Result_NotOpen,
        Result_BlobWriteFailed,
        Result_Error,
    };

    class Transaction;

    /**
     * デストラクタで自動的にcloseを行う
     */
    virtual ~Sqlite3Database();

    /**
     * データベースのバージョンを上書きする
     */
    Result_e updateSchemaVersion(const SqliteSchema *schema);

    /**
     * DB内部のバージョンと
     */
    bool hasMigrationRequest(const SqliteSchema *schema, int32_t *resultOldVersion);

    /**
     * Schemaに関連した全てのテーブルを生成する
     */
    Result_e createTable(const SqliteSchema *schema);

    Result_e createTable(const SqliteEntity *entity);

    /**
     * 事前に設定したBind情報に基いて、データを挿入する。
     * 完全に型安全なチェックはしないので、Bindの生成には注意すること。
     */
    Result_e insertOrUpdate(const SqliteEntity *entity, const void *dataStruct);

    /**
     * クエリを実行する。
     * queryはnullptrを許容する。
     */
    std::shared_ptr<Sqlite3Cursor> query(const SqliteEntity *entity, const SqliteEntity::Query *query, Result_e *result);

    /**
     * クエリを実行する。
     * 実行に失敗した場合はnullptrを返却し、resultにエラーコードを格納する。
     */
    std::shared_ptr<Sqlite3Cursor> query(const char *sql, Result_e *result);

    /**
     * トランザクションを開始する
     */
    std::shared_ptr<Transaction> beginTransaction();

    /**
     * SQL文を実行する
     */
    Result_e exec(const string &sql);

    /**
     * SQL文を実行する
     */
    Result_e exec(const char *sql);

    /**
     * 開いている場合はtrue
     * 既にcloseされている場合はtrue
     */
    bool isOpen() const;

    /**
     * sqliteを閉じる
     */
    void close();

    /**
     * データベースを開く
     */
    static std::shared_ptr<Sqlite3Database> open(const string &path, const OpenFlag_e flag);

    class Transaction {
    public:

        /**
         * 明示的にコミットする
         */
        void commit();

        ~Transaction();

    private:
        Transaction() = default;

        friend class Sqlite3Database;

        sp<internal::SqliteDbWrapper> impl;
    };

private:
    /**
     * 生成と同時に開く
     */
    Sqlite3Database();

    sp<internal::SqliteDbWrapper> impl;
};

}


