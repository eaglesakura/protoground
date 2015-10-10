#pragma once

#include "es/protoground.hpp"
#include "IDatabase.h"

namespace es {


class KeyValueStore : public virtual IDatabase {
public:
    KeyValueStore() = default;

    KeyValueStore(const Sqlite3Database::OpenFlag_e &openFlag, const string &path) : IDatabase(openFlag, path) { }

    int64_t get(const string &key, const int64_t def);

    double get(const string &key, const double def);

    string get(const string &key, const string &def);

    void put(const string &key, const int64_t value);

    void put(const string &key, const double value);

    void put(const string &key, const string &value);

    virtual ~KeyValueStore() = default;

protected:


    virtual std::shared_ptr<Sqlite3Database> openDatabase() override;

private:
    struct DB_KEY_VALUE_DATA {
        string KEY;
        string VALUE;
        ByteBuffer VALUE_BLOB;
        int64_t DATE;
    };

    static const SqliteSchema &getSchema();
};


}

