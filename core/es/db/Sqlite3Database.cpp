#include "Sqlite3Database.h"
#include <sqlite3.h>
#include <es/internal/log/Log.h>
#include <es/util/StringUtil.h>
#include "internal/SqliteObjectWrapper.hpp"
#include "schema/SqliteSchema.h"
#include "schema/SqliteEntity.h"

namespace es {

Sqlite3Database::Sqlite3Database() {
}

Sqlite3Database::~Sqlite3Database() {
    this->close();
}

bool Sqlite3Database::isOpen() const {
    return (bool) impl;
}


Sqlite3Database::Result_e Sqlite3Database::exec(const string &sql) {
    return exec(sql.c_str());
}

Sqlite3Database::Result_e Sqlite3Database::exec(const char *sql) {
    auto impl = this->impl;
    if (!impl || !impl->isOpen()) {
        return Result_NotOpen;
    }

    int error = 0;
    if ((error = sqlite3_exec(impl->db, sql, nullptr, nullptr, nullptr)) != SQLITE_OK) {
        eslog("Failed SQLITE :: %s", sql);
        // TODO 詳細なエラーコードを指定する
        return Result_Error;
    } else {
        return Result_OK;
    }
}


std::shared_ptr<Sqlite3Cursor> Sqlite3Database::query(const SqliteEntity *entity, const SqliteEntity::Query *query, Result_e *result) {
    auto impl = this->impl;
    if (!impl || !impl->isOpen()) {
        return sp<Sqlite3Cursor>();
    }

    assert(entity);
    auto sql = entity->querySql(query);
    return this->query(sql.c_str(), result);
}

std::shared_ptr<Sqlite3Cursor> Sqlite3Database::query(const char *sql, Sqlite3Database::Result_e *result) {
    auto impl = this->impl;
    if (!impl || !impl->isOpen()) {
        return sp<Sqlite3Cursor>();
    }

    int error = 0;
    sqlite3_stmt *cursor = nullptr;
    if ((error = sqlite3_prepare_v2(impl->db, sql, -1, &cursor, nullptr)) != SQLITE_OK) {
        eslog("Failed SQLITE :: %s", sql);
        if (cursor) {
            sqlite3_finalize(cursor);
            cursor = nullptr;
        }
        if (result) {
            // TODO 詳細なエラーコードを指定する
            *result = Result_Error;
        }
        return sp<Sqlite3Cursor>();
    } else {
        sp<internal::SqliteCursorWrapper> cursorWrapper(new internal::SqliteCursorWrapper(cursor, impl));
        sp<Sqlite3Cursor> resultCursor(new Sqlite3Cursor(cursorWrapper));
        if (result) {
            *result = Result_OK;
        }
        return resultCursor;
    }
}

Sqlite3Database::Result_e Sqlite3Database::insertOrUpdate(const SqliteEntity *entity, const void *dataStruct) {
    assert(entity);

    auto impl = this->impl;
    if (!impl || !impl->isOpen()) {
        return Result_NotOpen;
    }

    string sql = entity->insertOrReplaceSql((uint8_t *) dataStruct);

    sqlite3_stmt *cursor = nullptr;
    int code = sqlite3_prepare_v2(impl->db, sql.c_str(), -1, &cursor, nullptr);
    if (code != SQLITE_OK) {
        eslog("Failed SQLITE :: %s", sql.c_str());

        if (cursor) {
            sqlite3_finalize(cursor);
            cursor = nullptr;
        }
        return Result_Error;
    }

    internal::SqliteCursorWrapper cursorWrapper(cursor, impl);

    {
        int index = 0;
        for (const auto &prop : entity->properties) {
            const ByteBuffer *buffer = prop.bindBlob(dataStruct, entity->binds[index]);
            if (buffer) {
                if (!buffer->empty()) {
                    sqlite3_step(cursor);

                    code = sqlite3_bind_blob(cursor, index, buffer->get(), buffer->length(), SQLITE_TRANSIENT);
                    if (code != SQLITE_OK) {
                        eslog("Failed BLOB :: %s", sql.c_str());
                        return Result_BlobWriteFailed;
                    }
                }
            }

            ++index;
        }
        sqlite3_step(cursor);
    }

    return Result_OK;
}

void Sqlite3Database::close() {
    impl.reset();
}

std::shared_ptr<Sqlite3Database> Sqlite3Database::open(const string &path, const Sqlite3Database::OpenFlag_e flag) {

    int openFlag = 0;
    if (flag == OpenFlag_ReadOnly) {
        openFlag = SQLITE_OPEN_READONLY;
    } else if (flag == OpenFlag_ReadWrite) {
        openFlag = SQLITE_OPEN_READWRITE;
    }

    sp<internal::SqliteDbWrapper> impl(new internal::SqliteDbWrapper());
    int error = SQLITE_OK;
    if ((error = sqlite3_open_v2(path.c_str(), &impl->db, openFlag, nullptr)) != SQLITE_OK) {
        if (error == SQLITE_CANTOPEN) {
            error = sqlite3_open_v2(path.c_str(), &impl->db, SQLITE_OPEN_CREATE | openFlag, nullptr);
        }
    }
    if (error != SQLITE_OK || !impl->db) {
        return sp<Sqlite3Database>();
    }

    sp<Sqlite3Database> result(new Sqlite3Database());
    result->impl = impl;

    return result;
}

Sqlite3Database::Result_e Sqlite3Database::updateSchemaVersion(const SqliteSchema *schema) {

    SqliteSchema::Meta meta;
    meta.KEY = "VERSION";
    meta.VALUE = util::format("%d", schema->version);
    const auto &metaEntity = SqliteSchema::getMetaEntity();

    eslog("Update DatabaseVersion(%d)", schema->version);
    return insertOrUpdate(&metaEntity, &meta);
}

bool Sqlite3Database::hasMigrationRequest(const SqliteSchema *schema, int32_t *resultOldVersion) {
    assert(schema);
    assert(isOpen());

    const auto &metaEntity = SqliteSchema::getMetaEntity();
    if (exec(metaEntity.createTableSql()) != Result_OK) {
        return false;
    }

    SqliteEntity::Query q;
    q.and_equal(metaEntity.properties[0]/* KEY */, "VERSION");
    auto cursor = query(&metaEntity, &q, nullptr);
    if (!cursor || !cursor->next()) {
        // 初回データを投入する
        cursor.reset();

        if (resultOldVersion) {
            *resultOldVersion = 0;
        }
        return true;
    }

    // 読み込みが可能な場合、バージョンを取得する
    string dbVersionString;
    if (!cursor->load(1 /* VALUE */, &dbVersionString)) {
        // 読み込みに失敗したらマイグレーションが必要
        return true;
    }
    int64_t oldVersion = util::asInteger(dbVersionString);
    if ((int32_t) oldVersion != schema->version) {
        // アップデートが必要となる
        if (resultOldVersion) {
            *resultOldVersion = (int32_t) oldVersion;
        }
        return true;
    }

    return false;
}

void Sqlite3Database::Transaction::commit() {
    if (impl) {
        sqlite3_exec(impl->db, "COMMIT", nullptr, nullptr, nullptr);
        impl.reset();
    }
}

Sqlite3Database::Transaction::~Transaction() {
    commit();
}

std::shared_ptr<Sqlite3Database::Transaction> Sqlite3Database::beginTransaction() {
    auto impl = this->impl;
    if (!impl || !impl->isOpen()) {
        return sp<Transaction>();
    }

    if (exec("BEGIN") == Result_OK) {
        sp<Transaction> tr(new Transaction());
        tr->impl = impl;
        return tr;
    } else {
        return sp<Transaction>();
    }
}

Sqlite3Database::Result_e Sqlite3Database::createTable(const SqliteSchema *schema) {
    assert(schema);
    for (const auto &entity : schema->entities) {
        Result_e result;
        if ((result = createTable(&entity)) != Result_OK) {
            return result;
        }
    }
    return Result_OK;
}

Sqlite3Database::Result_e Sqlite3Database::createTable(const SqliteEntity *entity) {
    assert(entity);
    return exec(entity->createTableSql());
}
}