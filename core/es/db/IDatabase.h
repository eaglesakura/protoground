#pragma once

#include "es/protoground.hpp"
#include "es/db/Sqlite3Database.h"
#include <es/system/Thread.hpp>


namespace es {

/**
 * データベースの基本機能を構築する。
 *
 * 複数Threadからのアクセスに対応するため、必要に応じてMutexを確保する。
 */
class IDatabase : public virtual Object {
public:
    class Token;

    /**
     * DBへのアクセスを開始する。
     * DBがまだ開かれていない場合、openする。
     */
    virtual std::shared_ptr<Token> open();

    /**
     * トランザクションを開始する
     */
    virtual std::shared_ptr<Sqlite3Database::Transaction> beginTransaction();

    typedef std::function<void(IDatabase *db)> TransactionCallback;

    /**
     * トランザクション内でコールバックを実行する
     */
    virtual void runInTx(const TransactionCallback &callback);

    class Token : public virtual Object {
    public:
        ~Token();

    private:
        Token() = default;

        friend class IDatabase;

        IDatabase *self = nullptr;
    };


    const string &getPath() const;

    void setPath(const string &path);

    const Sqlite3Database::OpenFlag_e &getOpenFlag() const;

    void setOpenFlag(const Sqlite3Database::OpenFlag_e &openFlag);

    virtual ~IDatabase();

protected:
    IDatabase(const Sqlite3Database::OpenFlag_e &openFlag, const string &path);

    IDatabase();


    virtual std::shared_ptr<Sqlite3Database> openDatabase() = 0;

    std::shared_ptr<Sqlite3Database> db;

private:
    /**
     * 全てのDBのアクセスが解放されるまでロックする
     */
    void waitDatabaseClosing();

    /**
     * データベースを閉じる。
     * 通常、Tokenから自動的に閉じられる。
     */
    void close();

    Sqlite3Database::OpenFlag_e openFlag = Sqlite3Database::OpenFlag_ReadWrite;
    string path;

    mutex mtx;
    std::atomic<int32_t> refs;
};
}