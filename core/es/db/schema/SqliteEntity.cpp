#include "SqliteEntity.h"
#include "es/util/StringUtil.h"

namespace es {

string SqliteEntity::Property::toString(const void *addr) const {
    assert(type != Type_Unknown);

    switch (type) {
        case Type_Int64:
            return util::format("%lld", *((int64_t *) addr));
        case Type_Double:
            return util::format("%f", *((double *) addr));
        case Type_String:
            return string("'") + *((string *) addr) + string("'");
        case Type_Blob:
            return string("?");
        default:
            assert(false);
    }
    return string("");
}

string SqliteEntity::Property::createTableSql() const {
    string result(" '");
    result += name;
    result += string("'");

    // 型指定
    {
        const char *const TYPE_TABLE[] = {
                " INTEGER ",
                " REAL ",
                " TEXT ",
                " BLOB "
        };
        assert(type != Type_Unknown);
        result += TYPE_TABLE[type];
    }

    // フラグ指定
    if ((flags & Flag_PrimaryKey) == Flag_PrimaryKey) {
        result += "PRIMARY KEY ";
    }
    if (flags & Flag_NotNull) {
        result += "NOT NULL ";
    }
    if (flags & Flag_Unique) {
        result += " UNIQUE ";
    }

    return result;
}

string SqliteEntity::createTableSql() const {
    string result("CREATE TABLE IF NOT EXISTS '");
    result += name;
    result += "' (";

    int index = 0;
    for (const auto &prop : properties) {
        if (index > 0) {
            result += ", ";
        }
        result += prop.createTableSql();
        ++index;
    }
    result += ");";

    return result;
}

string SqliteEntity::dropTableSql() const {
    string result("DROP TABLE IF NOT EXISTS '");
    result += name;
    result += "'";
    return result;
}

string SqliteEntity::querySql(const SqliteEntity::Query *query) const {
    string result("SELECT");

    {
        int index = 0;
        for (const auto &prop : properties) {
            if (index > 0) {
                result += ", ";
            } else {
                result += " ";
            }
            result += prop.name;
            ++index;
        }
    }

    {
        result += " FROM ";
        result += name;
    }
    if (query) {
        if (!query->where.empty()) {
            result += " WHERE ";
            result += query->where;
        }
    }
    result += ";";
    return result;
}

SqliteEntity::Query &SqliteEntity::Query::and_equal(const SqliteEntity::Property &prop, const string &value) {
    if (!where.empty()) {
        where += "AND ";
    }
    where += prop.name;
    where += "='";
    where += wrapInjection(value);
    where += "' ";

    return *this;
}

SqliteEntity::Query &SqliteEntity::Query::and_equal(const SqliteEntity::Property &prop, const int64_t &value) {
    if (!where.empty()) {
        where += "AND ";
    }
    where += prop.name;
    where += "=";
    where += util::format("%d", value);
    where += " ";

    return *this;
}

SqliteEntity::Query &SqliteEntity::Query::and_equal(const SqliteEntity::Property &prop, const double &value) {
    if (!where.empty()) {
        where += "AND ";
    }
    where += prop.name;
    where += "=";
    where += util::format("%f", value);
    where += " ";
    return *this;
}


SqliteEntity::Query &SqliteEntity::Query::and_like(const SqliteEntity::Property &prop, const string &value) {
    if (!where.empty()) {
        where += "AND ";
    }
    where += prop.name;
    where += " LIKE '";
    where += wrapInjection(value);
    where += "'";
    return *this;
}

string SqliteEntity::Query::wrapInjection(const string &value) {
    return value;
}

SqliteEntity::Bind SqliteEntity::Bind::make(const SqliteEntity::Property &prop, const void *headAddr, const int64_t *addr) {
    assert(prop.type == Property::Type_Int64);

    Bind bind;
    bind.offset = ((uint8_t *) addr) - ((uint8_t *) headAddr);
    return bind;
}

SqliteEntity::Bind SqliteEntity::Bind::make(const SqliteEntity::Property &prop, const void *headAddr, const double *addr) {
    assert(prop.type == Property::Type_Double);
    Bind bind;
    bind.offset = ((uint8_t *) addr) - ((uint8_t *) headAddr);
    return bind;
}

SqliteEntity::Bind SqliteEntity::Bind::make(const SqliteEntity::Property &prop, const void *headAddr, const string *addr) {
    assert(prop.type == Property::Type_String);
    Bind bind;
    bind.offset = ((uint8_t *) addr) - ((uint8_t *) headAddr);
    return bind;
}

SqliteEntity::Bind SqliteEntity::Bind::make(const SqliteEntity::Property &prop, const void *headAddr, const ByteBuffer *addr) {
    assert(prop.type == Property::Type_Blob);
    Bind bind;
    bind.offset = ((uint8_t *) addr) - ((uint8_t *) headAddr);
    return bind;
}

string SqliteEntity::insertOrReplaceSql(const uint8_t *head) const {
    assert(properties.size() == binds.size());
    assert(head);

    string result("INSERT OR REPLACE INTO ");

    // target
    {
        result += name;
        result += "(";
        int index = 0;
        for (const auto &prop  : properties) {
            if (index > 0) {
                result += ",";
            }
            result += prop.name;
            ++index;
        }
        result += ")";
    }


    {
        result += " VALUES(";
        int index = 0;
        for (const auto &bind : binds) {
            if (index > 0) {
                result += ",";
            }

            result += properties[index].toString(head + bind.offset);

            ++index;
        }
        result += ")";
    }

    return result;

}

const ByteBuffer *SqliteEntity::Property::bindBlob(const void *head, const SqliteEntity::Bind &bind) const {
    if (type == Type_Blob) {
        return (ByteBuffer *) Buffer::offsetBytes(head, bind.offset);
    }
    return nullptr;
}

SqliteEntity &SqliteEntity::addProperty(const SqliteEntity::Property &prop, const void *headAddr, const int64_t *value) {
    properties.push_back(prop);
    binds.push_back(Bind::make(prop, headAddr, value));
    return *this;
}

SqliteEntity &SqliteEntity::addProperty(const SqliteEntity::Property &prop, const void *headAddr, const double *value) {
    properties.push_back(prop);
    binds.push_back(Bind::make(prop, headAddr, value));
    return *this;
}

SqliteEntity &SqliteEntity::addProperty(const SqliteEntity::Property &prop, const void *headAddr, const string *value) {
    properties.push_back(prop);
    binds.push_back(Bind::make(prop, headAddr, value));
    return *this;
}

SqliteEntity &SqliteEntity::addProperty(const SqliteEntity::Property &prop, const void *headAddr, const ByteBuffer *value) {
    properties.push_back(prop);
    binds.push_back(Bind::make(prop, headAddr, value));
    return *this;
}

SqliteEntity::Property::Type_e SqliteEntity::Property::toType(const int64_t &value) {
    return Type_Int64;
}

SqliteEntity::Property::Type_e SqliteEntity::Property::toType(const double &value) {
    return Type_Double;
}

SqliteEntity::Property::Type_e SqliteEntity::Property::toType(const string &value) {
    return Type_String;
}

SqliteEntity::Property::Type_e SqliteEntity::Property::toType(const ByteBuffer &value) {
    return Type_Blob;
}

const SqliteEntity::Property &SqliteEntity::getProperty(const uint32_t index) const {
    return properties[index];
}

const SqliteEntity::Property &SqliteEntity::getProperty(const string &name) const {
    for (const auto &prop : properties) {
        if (prop.name == name) {
            return prop;
        }
    }
    assert(false);
    return properties[0];
}
}