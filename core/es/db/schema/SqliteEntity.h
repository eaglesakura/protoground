#pragma once

#include "es/protoground.hpp"
#include <vector>
#include <es/memory/Buffer.hpp>

namespace es {

class Sqlite3Database;

struct SqliteSchema;

class Sqlite3Cursor;

struct SqliteEntity {
    struct Bind;

    struct Property {
        /**
         * プロパティ名
         */
        string name;

        enum Type_e {
            Type_Int64 = 0,
            Type_Double,
            Type_String,
            Type_Blob,
            Type_Unknown,
        } type = Type_Unknown;

        /**
         * プロパティの制御フラグ
         */
        uint32_t flags = 0;

        /**
         * プロパティの制御フラグ
         */
        enum Flag_e {
            Flag_NotNull = 0x01 << 1,
            Flag_Unique = 0x01 << 2,
            Flag_Index = 0x01 << 3,

            Flag_PrimaryKey = (0x01 << 0) | Flag_NotNull | Flag_Unique | Flag_Index,
        };


        Property() = default;


        Property(const string &name, const Type_e &type, uint32_t flags) : name(name), type(type), flags(flags) { }

        Property(const char *name, const Type_e &type, uint32_t flags) : name(name), type(type), flags(flags) { }

        static Type_e toType(const int64_t &value);

        static Type_e toType(const double &value);

        static Type_e toType(const string &value);

        static Type_e toType(const ByteBuffer &value);

    private:
        friend struct SqliteEntity;

        friend class Sqlite3Database;

        friend class Sqlite3Cursor;

        string toString(const void *addr) const;

        /**
         * Blobデータであれば、ポインタを取得する。
         * それ以外であれば、nullptrを返却する。
         */
        const ByteBuffer *bindBlob(const void *head, const Bind &bind) const;

        string createTableSql() const;
    };

    /**
     * クエリを発行する
     */
    struct Query {
        Query &and_equal(const Property &prop, const string &value);

        Query &and_equal(const Property &prop, const int64_t &value);

        Query &and_equal(const Property &prop, const double &value);

        Query &and_like(const Property &prop, const string &value);

        /**
         * インジェクション対策の置換を行う
         */
        static string wrapInjection(const string &value);

    private:
        string where;

        friend class Sqlite3Database;

        friend struct SqliteEntity;
    };

    /**
     * 構造体のメモリアドレスとpropertyを関連付ける
     * 型が違っている場合、assertによって停止をかける。
     */
    struct Bind {

        static Bind make(const Property &prop, const void *headAddr, const int64_t *addr);

        static Bind make(const Property &prop, const void *headAddr, const double *addr);

        static Bind make(const Property &prop, const void *headAddr, const string *addr);

        static Bind make(const Property &prop, const void *headAddr, const ByteBuffer *addr);

    private:
        friend struct SqliteEntity;

        friend class Sqlite3Cursor;


        uint32_t offset;
    };

    /**
     * エンティティ名
     */
    string name;

    /**
     * 管理されるプロパティ一覧
     */
    std::vector<Property> properties;

    /**
     * 構造体とのバインディング情報
     */
    std::vector<Bind> binds;


    SqliteEntity() = default;

    SqliteEntity(const string &name) : name(name) { }

    /**
     * プロパティを追加する
     */
    SqliteEntity &addProperty(const Property &prop, const void *headAddr, const int64_t *value);

    /**
     * プロパティを追加する
     */
    SqliteEntity &addProperty(const Property &prop, const void *headAddr, const double *value);

    /**
     * プロパティを追加する
     */
    SqliteEntity &addProperty(const Property &prop, const void *headAddr, const string *value);

    /**
     * プロパティを追加する
     */
    SqliteEntity &addProperty(const Property &prop, const void *headAddr, const ByteBuffer *value);

    const Property &getProperty(const uint32_t index) const;

    const Property &getProperty(const string &name) const;

private:
    friend struct SqliteSchema;

    friend class Sqlite3Database;

    /**
     * テーブル作成用SQL文を生成する
     */
    string createTableSql() const;

    /**
     * テーブル排気用SQL文を生成する
     */
    string dropTableSql() const;

    /**
     * クエリ用のSQL文を生成する
     */
    string querySql(const Query *query) const;

    /**
     * データ挿入用のSQL文を生成する
     */
    string insertOrReplaceSql(const uint8_t *head) const;
};

}
