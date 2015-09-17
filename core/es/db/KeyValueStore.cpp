#include <iosfwd>
#include <memory>
#include <es/protoground.hpp>
#include <es/db/schema/SqliteSchema.h>
#include <es/util/StringUtil.h>
#include <es/system/NanoTimer.h>
#include <es/internal/log/Log.h>
#include "KeyValueStore.h"
#include "es/db/schema/SqliteSchemaSupport.hpp"

namespace es {

std::shared_ptr<Sqlite3Database> KeyValueStore::openDatabase() {
    auto db = Sqlite3Database::open(getPath(), getOpenFlag());
    if (db) {
        const auto &schema = getSchema();
        int32_t oldVeresion = 0;
        if (db->hasMigrationRequest(&schema, &oldVeresion)) {
            if (oldVeresion == 0) {
                db->createTable(&schema);
            }
        }
    }

    return db;
}


int64_t KeyValueStore::get(const string &key, const int64_t def) {
    string value = get(key, "");
    if (value.empty()) {
        return def;
    } else {
        return util::asInteger(value);
    }
}

double KeyValueStore::get(const string &key, const double def) {
    string value = get(key, "");
    if (value.empty()) {
        return def;
    } else {
        return util::asDouble(value);
    }
}

string KeyValueStore::get(const string &key, const string &def) {
    const auto &schema = getSchema();
    const auto &entity = schema.entities[0];

    SqliteEntity::Query query;
    query.and_equal(entity.properties[0] /* KEY */, key);

    auto cursor = db->query(&schema.entities[0], &query, nullptr);
    if (!cursor || !cursor->next()) {
        return def;
    } else {
        DB_KEY_VALUE_DATA data;
        cursor->loadToStruct(&schema.entities[0], &data);
        return data.VALUE;
    }
}

void KeyValueStore::put(const string &key, const int64_t value) {
    put(key, util::format("%lld", value));
}

void KeyValueStore::put(const string &key, const double value) {
    put(key, util::format("%f", value));
}

void KeyValueStore::put(const string &key, const string &value) {
    DB_KEY_VALUE_DATA data;
    data.KEY = key;
    data.VALUE = value;
    data.VALUE_BLOB = ByteBuffer();
    data.DATE = NanoTimer::systemMilliSeconds();

    const auto &schema = getSchema();
    if (db->insertOrUpdate(&schema.entities[0], &data) != Sqlite3Database::Result_OK) {
        eslog("KeyValue insert error key(%s) value(%s)", key.c_str(), value.c_str());
    }
}

const SqliteSchema &KeyValueStore::getSchema() {
    static PGD_SQLITE_SCHEMA_MAKE(0x01);
    if (PGD_SQLITE_SCHEMA_REQUEST_INITIALIZE) {
        PGD_SQLITE_ENTITY_MAKE(DB_KEY_VALUE_DATA);

        PGD_SQLITE_ENTITY_PROPERTY(DB_KEY_VALUE_DATA, KEY, ::es::SqliteEntity::Property::Flag_PrimaryKey);
        PGD_SQLITE_ENTITY_PROPERTY(DB_KEY_VALUE_DATA, VALUE, 0);
        PGD_SQLITE_ENTITY_PROPERTY(DB_KEY_VALUE_DATA, VALUE_BLOB, 0);
        PGD_SQLITE_ENTITY_PROPERTY(DB_KEY_VALUE_DATA, DATE, ::es::SqliteEntity::Property::Flag_Index);


        PGD_SQLITE_SCHEMA_ADD_ENTITY(DB_KEY_VALUE_DATA);
    }
    return PGD_SQLITE_SCHEMA;
}
}