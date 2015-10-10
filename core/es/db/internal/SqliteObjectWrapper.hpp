#pragma once

#include "es/protoground.hpp"
#include <sqlite3.h>

namespace es {
namespace internal {

struct SqliteDbWrapper {
    sqlite3 *db = nullptr;

    bool isOpen() {
        return db != nullptr;
    }

    ~SqliteDbWrapper() {
        if (db) {
            sqlite3_close_v2(db);
            db = nullptr;
        }
    }
};

struct SqliteCursorWrapper {
    sqlite3_stmt *cursor = nullptr;

    std::shared_ptr<SqliteDbWrapper> db;


    SqliteCursorWrapper(sqlite3_stmt *cursor, const std::shared_ptr<SqliteDbWrapper> &db) : cursor(cursor), db(db) {
        assert(cursor);
        assert(db);
        assert(db->isOpen());
    }

    ~SqliteCursorWrapper() {
        if (cursor) {
            sqlite3_finalize(cursor);
            cursor = nullptr;
        }
        db.reset();
    }
};

}
}