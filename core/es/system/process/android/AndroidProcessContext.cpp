#ifdef __ANDROID__

#include "es/internal/protoground-internal.hpp"
#include "AndroidProcessContext.h"
#include "es/asset/AssetManager.h"

#include "es/android/internal/asset/AndroidAssetLoader.h"
#include "es/system/string/internal/JavaStringConverterImpl.h"

using namespace jc;
using namespace jc::lang;

#ifndef ES_JNIONLOAD_DISABLE

extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    es::AndroidProcessContext::onBootProcess(vm);
    return JNI_VERSION_1_6;
}

};
#endif /* DISABLE */

namespace es {

namespace {
std::shared_ptr<AndroidProcessContext> gInstance;
}

class AndroidProcessContext::Impl {
public:
    JavaVM *vm;

    struct {
        struct {
            jc::lang::class_wrapper clazz;
            jc::lang::object_wrapper obj;
            jmethodID method_getAssets = nullptr;
            jmethodID method_getApplicationContext = nullptr;
        } application;

        struct {
            jc::lang::class_wrapper clazz;
            jc::lang::object_wrapper obj;
        } assetManager;

    } android;

    struct {
        struct {
            jc::lang::class_wrapper clazz;
            jmethodID method_newThread = nullptr;
        } processContextImpl;
    } protoground;


    Impl(JavaVM *newVM) : vm(newVM) {
        assert(vm);

    }

    virtual ~Impl() {
    }
};

AndroidProcessContext::AndroidProcessContext() {
}

AndroidProcessContext::~AndroidProcessContext() {
}

JavaVM *AndroidProcessContext::getJavaVM() const {
    return impl->vm;
}

JNIEnv *AndroidProcessContext::getThreadEnv() const {
    JNIEnv *result = nullptr;
    getJavaVM()->GetEnv((void **) &result, JNI_VERSION_1_6);
    assert(result);
    return result;
}

IProcessContext::PlatformType_e AndroidProcessContext::getPlatform() {
    return PlatformType_Android;
}

void AndroidProcessContext::onBootProcess(JavaVM *vm) {
    if (gInstance) {
        return;
    }

    gInstance.reset(new AndroidProcessContext());
    gInstance->impl.reset(new AndroidProcessContext::Impl(vm));
    gInstance->stringConverter.reset(new internal::JavaStringConverterImpl());
}

std::shared_ptr<IProcessContext> IProcessContext::getInstance() {
    if (!gInstance) {
        eslog("Call!! AndroidProcessContext::onBootProcess()");
        assert(false);
    }
    return gInstance;
}

jc::lang::object_wrapper AndroidProcessContext::getApplicationContext() const {
    return impl->android.application.obj;
}

void AndroidProcessContext::onCreateApplication(JNIEnv *env, jobject context) {
    // 互換性のため、プロセスを初期化する
    {
        JavaVM *vm = nullptr;
        env->GetJavaVM(&vm);
        onBootProcess(vm);
    }

    // JointConnector System更新
    ::jc::jni::initJniSupport(env);

    std::shared_ptr<Impl> impl = gInstance->impl;

    // Appliationクラスの初期化
    {
        impl->android.application.clazz = class_wrapper::find(env, "android/app/Application");
        assert(impl->android.application.clazz.hasObject());
        impl->android.application.clazz.globalRef().multiThread(true);

        impl->android.application.method_getAssets = impl->android.application.clazz.getMethod("getAssets", "()Landroid/content/res/AssetManager;", false);
        impl->android.application.method_getApplicationContext = impl->android.application.clazz.getMethod("getApplicationContext", "()Landroid/content/Context;", false);
        assert(impl->android.application.method_getAssets);
        assert(impl->android.application.method_getApplicationContext);

        // applicationを設定する
        impl->android.application.obj = object_wrapper(env->CallObjectMethod(context, impl->android.application.method_getApplicationContext), env, false);
        assert(impl->android.application.obj.hasObject());
        impl->android.application.obj.globalRef().multiThread(true);
    }

    // AssetManagerクラスの初期化
    {
        impl->android.assetManager.clazz = class_wrapper::find(env, "android/content/res/AssetManager");
        assert(impl->android.assetManager.clazz.hasObject());
        impl->android.assetManager.clazz.globalRef().multiThread(true);

        impl->android.assetManager.obj = object_wrapper(env->CallObjectMethod(context, impl->android.application.method_getAssets), env, false);
        assert(impl->android.assetManager.obj.hasObject());
        impl->android.assetManager.obj.globalRef().setMultiThreadAccess(true);

    }

    // プロセス管理用のUtilクラスを追加
    {
        impl->protoground.processContextImpl.clazz = class_wrapper::find(env, "com/eaglesakura/protoground/ProcessContextImpl");
        assert(impl->protoground.processContextImpl.clazz.hasObject());
        impl->protoground.processContextImpl.clazz.globalRef().multiThread(true);

        // newThread
        impl->protoground.processContextImpl.method_newThread =
                impl->protoground.processContextImpl.clazz.getMethod("newThread", "(Ljava/lang/String;JJ)V", true);
        assert(impl->protoground.processContextImpl.method_newThread);

    }

    // デフォルトのアセットを追加する
    {
        gInstance->assetManager->addLoader(std::shared_ptr<IAssetLoader>(new internal::AndroidAssetLoader(impl->android.assetManager.obj)));
    }
}

namespace {

typedef void (*AndroidProcessContext_ThreadCallback)(void *ptr);

struct AndroidProcessContext_ThreadCallbackArg {
    IProcessContext::ThreadCallback callback;
};

static void AndroidProcessContext_newThread(AndroidProcessContext_ThreadCallbackArg *arg) {
    assert(arg);

    arg->callback();

    es::util::safe_delete(&arg);
    assert(!arg);
}

}

void AndroidProcessContext::newThread(const std::string &name, const IProcessContext::ThreadCallback callback) {
    AndroidProcessContext_ThreadCallbackArg *arg = new AndroidProcessContext_ThreadCallbackArg();
    arg->callback = callback;

    JNIEnv *env = impl->protoground.processContextImpl.clazz.getEnv();
    assert(env);

    jc::lang::string_wrapper jstr(name.c_str(), env);


    env->CallStaticVoidMethod(
            impl->protoground.processContextImpl.clazz.getJclass(),
            impl->protoground.processContextImpl.method_newThread,
            jstr.getJstring(),
            (jlong) (es::AndroidProcessContext_newThread),
            (jlong) (arg)
    );
}

}

extern "C" {

JNIEXPORT void JNICALL Java_com_eaglesakura_protoground_ProcessContextImpl_newThreadCall(JNIEnv *env, jlong funcPtr, jlong funcArg) {
    ((es::AndroidProcessContext_ThreadCallback) funcPtr)((void *) funcArg);
}

}


#endif /* __ANDROID__ */