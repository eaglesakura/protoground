#pragma once

#include "estest/protoground-test.hpp"
#include <sqlite3.h>
#include <es/db/schema/SqliteSchema.h>
#include <es/system/NanoTimer.h>
#include <es/db/KeyValueStore.h>
#include "es/db/Sqlite3Database.h"
#include "es/db/schema/SqliteSchemaSupport.hpp"

namespace es {
namespace test {

static const char *TESTING_DATABASE_NAME = "shared/storage/test.db";

//TEST(Sqlite3Raw, DbOpenWrapper) {
//    auto db = Sqlite3Database::open(TESTING_DATABASE_NAME, Sqlite3Database::OpenFlag_ReadWrite);
//    ASSERT_TRUE((bool) db);
//    ASSERT_TRUE(db->isOpen());
//
////    ASSERT_EQ(db->exec("create table user (id integer, age integer)"), Sqlite3Database::Result_OK);
//
//    db->close();
//    ASSERT_FALSE(db->isOpen());
//}

TEST(Sqlite3Raw, CreateTable) {
    auto db = Sqlite3Database::open(TESTING_DATABASE_NAME, Sqlite3Database::OpenFlag_ReadWrite);
    ASSERT_TRUE((bool) db);
    ASSERT_TRUE(db->isOpen());

    SqliteSchema schema;

    if (db->hasMigrationRequest(&schema, nullptr)) {
        eslog("Initial hasMigrationRequest");
    }

    ++schema.version;
    // バージョンアップしたらマイグレーションリクエストが発生しなければならない
    ASSERT_TRUE(db->hasMigrationRequest(&schema, nullptr));
    // マイグレーション後にDBのアップグレードし、マイグレーションが完了したことを確認
    ASSERT_EQ(db->updateSchemaVersion(&schema), Sqlite3Database::Result_OK);
    ASSERT_FALSE(db->hasMigrationRequest(&schema, nullptr));


    // バージョンを元に戻す
    schema.version = 0x01;
    ASSERT_EQ(db->updateSchemaVersion(&schema), Sqlite3Database::Result_OK);
}

TEST(Sqlite, KeyValueStore) {
    KeyValueStore kvs(Sqlite3Database::OpenFlag_ReadWrite, "shared/storage/kvs.db");

    {
        auto token = kvs.open();

        kvs.put("IntValue", (int64_t) 12345678);
        kvs.put("DoubleValue", 1.23456);
        kvs.put("stringValue", "str");
    }
    {
        auto token = kvs.open();

        ASSERT_EQ(kvs.get("IntValue", (int64_t) 0), 12345678);
        ASSERT_TRUE(es::equals((float) kvs.get("DoubleValue", 0.0), (float) 1.23456));
        ASSERT_EQ(kvs.get("stringValue", ""), "str");
    }
}


TEST(Crock, SystemTimeMs) {
    eslog("SystemTime(%llu)", NanoTimer::systemMilliSeconds());
}

}
}