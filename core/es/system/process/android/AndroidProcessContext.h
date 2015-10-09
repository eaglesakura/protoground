#ifdef __ANDROID__

#pragma once

#include <es/memory/query_ptr.hpp>
#include "es/system/process/IProcessContext.h"
#include "JointConnector.hpp"

namespace es {

class AndroidProcessContext : public IProcessContext {
public:
    typedef query_ptr<AndroidProcessContext, InterfaceId_Android_ProcessContext> query;

    virtual ~AndroidProcessContext();

    /**
     * JavaVMを取得する
     */
    JavaVM *getJavaVM() const;

    /**
     * 現在のThreadに紐付いたJNIEnvを取得する
     */
    JNIEnv *getThreadEnv() const;

    /**
     * Appliation情報を取得する
     */
    jc::lang::object_wrapper getApplicationContext() const;

    /**
     * apkがデバッグ状態でビルドされていればtrue
     */
    bool isApkDebuggalbe() const;

    /**
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform() const override;


    virtual void newThread(const std::string &name, const ThreadCallback callback) override;

    /**
     * Databaseをopenするためのpathを取得する
     */
    virtual string getDatabasePath(const string &basePath) const;


    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override;

    /**
     * プロセス初期化を行う
     */
    static void onBootProcess(JavaVM *vm);

    /**
     * Application.onCreate後に呼び出す。
     *
     * ContextはApplicationContextである必要はなく、このメソッドは必ずgetApplication()で内部のContextを利用する。
     */
    static void onCreateApplication(JNIEnv *env, jobject context);

private:
    class Impl;

    std::shared_ptr<Impl> impl;

    AndroidProcessContext();
};

}

#endif /* __ANDROID__ */
