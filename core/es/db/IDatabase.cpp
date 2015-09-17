#include "IDatabase.h"

namespace es {

IDatabase::IDatabase(const Sqlite3Database::OpenFlag_e &openFlag, const string &path) : openFlag(openFlag), path(path), refs(0) {
}

IDatabase::IDatabase() : refs(0) {

}

void IDatabase::waitDatabaseClosing() {
    while (refs != 0) {
        // DBの使用が完了するまでスピンロック
        assert(refs >= 0);
    }
}

std::shared_ptr<IDatabase::Token> IDatabase::open() {
    if (refs == 0) {
        mutex_lock lock(mtx);
        if (refs == 0) {
            db = openDatabase();
        }

        if (!db) {
            return sp<Token>();
        }

        ++refs;
    } else {
        ++refs;
    }

    sp<Token> token(new Token());
    token->self = this;
    return token;
}

void IDatabase::close() {

    if ((--refs) == 0) {
        mutex_lock lock(mtx);
        if (refs != 0) {
            return;
        }

        // dbが利用し終わったのでcloseする
        db.reset();
    }
}


IDatabase::~IDatabase() {
    waitDatabaseClosing();
}

IDatabase::Token::~Token() {
    self->close();
}

std::shared_ptr<Sqlite3Database::Transaction> IDatabase::beginTransaction() {
    return db->beginTransaction();
}

void IDatabase::runInTx(const IDatabase::TransactionCallback &callback) {
    auto transaction = db->beginTransaction();
    callback(this);
    transaction->commit();
}

const string &IDatabase::getPath() const {
    return path;
}

void IDatabase::setPath(const string &path) {
    IDatabase::path = path;
}

const Sqlite3Database::OpenFlag_e &IDatabase::getOpenFlag() const {
    return openFlag;
}

void IDatabase::setOpenFlag(const Sqlite3Database::OpenFlag_e &openFlag) {
    IDatabase::openFlag = openFlag;
}

}