#pragma once

#include "es/protoground.hpp"
#include "SqliteEntity.h"

namespace es {

class Sqlite3Database;

struct SqliteSchema {
    /**
     * Schemaバージョン
     *
     * 標準は0x01
     */
    int32_t version = 0x01;

    /**
     * 管理されるエンティティ一覧
     */
    std::vector<SqliteEntity> entities;


    SqliteSchema() { }

    SqliteSchema(int32_t version) : version(version) { }

    SqliteSchema &addEntity(const SqliteEntity &entity);

    bool  hasEntity() const;

private:
    friend class Sqlite3Database;

    struct Meta {
        string KEY;
        string VALUE;
    };

    /**
     * メタ情報を記録するEntityを取得する
     */
    static const SqliteEntity &getMetaEntity();
};


}

