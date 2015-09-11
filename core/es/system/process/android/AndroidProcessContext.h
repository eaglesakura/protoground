#ifdef __ANDROID__

#pragma once

#include "es/system/process/IProcessContext.h"
#include "JointConnector.hpp"

namespace es {

class AndroidProcessContext : public IProcessContext {
public:
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
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform() override;


    virtual void newThread(const std::string &name, const ThreadCallback callback) override;

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
