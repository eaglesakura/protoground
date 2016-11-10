﻿#ifdef __ANDROID__

#include <es/system/string/internal/CucharStringConverter.hpp>
#include "es/internal/protoground-internal.hpp"
#include "AndroidProcessContext.h"
#include "es/asset/AssetManager.h"

#include "es/android/internal/asset/AndroidAssetLoader.h"

#if __ANDROID_API__ < 20
// not support(locale.so)
#include "es/system/string/internal/JavaStringConverterImpl.hpp"
#define PGD_LOCALE_NOTSUPPORT 1
#else

#include "es/system/string/internal/CucharStringConverter.hpp"

#endif

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
            jmethodID method_getDatabasePath = nullptr;
            jmethodID method_isAndroidDebugable = nullptr;
            jmethodID method_getSdkVersion = nullptr;
            jmethodID method_getEndianCheckCode = nullptr;
        } processContextImpl;
    } protoground;

    mutable IProcessContext::VersionInfo versionCache;

    Endian_e endianCache;

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

IProcessContext::PlatformType_e AndroidProcessContext::getPlatform() const {
    return PlatformType_Android;
}

void AndroidProcessContext::onBootProcess(JavaVM *vm) {
    if (gInstance) {
        return;
    }

    gInstance.reset(new AndroidProcessContext());
    gInstance->impl.reset(new AndroidProcessContext::Impl(vm));
#if defined(PGD_LOCALE_NOTSUPPORT)
    gInstance->stringConverter.reset(new internal::JavaStringConverterImpl());
#else
    gInstance->stringConverter.reset(new internal::CucharStringConverter());
#endif
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

        // getSdkVersion
        impl->protoground.processContextImpl.method_getEndianCheckCode =
                impl->protoground.processContextImpl.clazz.getMethod("getEndianCheckCode", "()I", true);
        assert(impl->protoground.processContextImpl.method_getEndianCheckCode);

        // getSdkVersion
        impl->protoground.processContextImpl.method_getSdkVersion =
                impl->protoground.processContextImpl.clazz.getMethod("getSdkVersion", "()I", true);
        assert(impl->protoground.processContextImpl.method_getSdkVersion);

        // getDatabasePath
        impl->protoground.processContextImpl.method_getDatabasePath =
                impl->protoground.processContextImpl.clazz.getMethod("getDatabasePath", "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;", true);
        assert(impl->protoground.processContextImpl.method_getDatabasePath);

        // newThread
        impl->protoground.processContextImpl.method_newThread =
                impl->protoground.processContextImpl.clazz.getMethod("newThread", "(Ljava/lang/String;JJ)V", true);
        assert(impl->protoground.processContextImpl.method_newThread);

        // debugable
        impl->protoground.processContextImpl.method_isAndroidDebugable =
                impl->protoground.processContextImpl.clazz.getMethod("isAndroidDebugable", "(Landroid/content/Context;)Z", true);
        assert(impl->protoground.processContextImpl.method_isAndroidDebugable);


    }

    // エンディアン判定を行う
    {
        const jint checkCode = env->CallStaticIntMethod(
                impl->protoground.processContextImpl.clazz.getJclass(),
                impl->protoground.processContextImpl.method_getEndianCheckCode
        );

        if (((uint8_t *) (&checkCode))[0] == 0x00) {
            impl->endianCache = Endian_Big;
        } else {
            impl->endianCache = Endian_Little;
        }
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

string AndroidProcessContext::getDatabasePath(const string &basePath) const {
    JNIEnv *env = impl->protoground.processContextImpl.clazz.getEnv();
    assert(env);

    jc::lang::string_wrapper jBasePath(basePath.c_str(), env);
    jobject fullPath = env->CallStaticObjectMethod(
            impl->protoground.processContextImpl.clazz.getJclass(),
            impl->protoground.processContextImpl.method_getDatabasePath,
            impl->android.application.obj.getJobject(),
            jBasePath.getJstring()
    );
    if (!fullPath) {
        // パスを正常に取得できなかった
        return string();
    } else {
        // パスを返す
        jc::lang::string_wrapper jFullPath(fullPath, env, false);
        return jFullPath.asString();
    }
}

IProcessContext::Endian_e AndroidProcessContext::getVirtualMachineEndian() const {
    return impl->endianCache;
}

Object::QueryResult_e AndroidProcessContext::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    PGD_SUPPORT_QUERY(InterfaceId_Android_ProcessContext, AndroidProcessContext);
    return Object::queryInterface(interfaceId, resultInterfacePtr);
}

bool AndroidProcessContext::isApkDebuggalbe() const {
    JNIEnv *env = impl->protoground.processContextImpl.clazz.getEnv();
    assert(env);
    return env->CallStaticBooleanMethod(
            impl->protoground.processContextImpl.clazz.getJclass(),
            impl->protoground.processContextImpl.method_isAndroidDebugable,
            impl->android.application.obj.getJobject()
    );
}

bool AndroidProcessContext::getPlatformVersion(IProcessContext::VersionInfo *result) const {
    if (impl->versionCache.major) {
        *result = impl->versionCache;
        return true;
    }

    JNIEnv *env = impl->protoground.processContextImpl.clazz.getEnv();
    assert(env);

    const int sdkInt = env->CallStaticIntMethod(
            impl->protoground.processContextImpl.clazz.getJclass(),
            impl->protoground.processContextImpl.method_getSdkVersion
    );

    switch (sdkInt) {
        case 14:
            result->major = 4;
            result->minor = 0;
            result->displayName = "Android 4.0";
            break;
        case 15:
            result->major = 4;
            result->minor = 0;
            result->displayName = "Android 4.0.3";
            break;
        case 16:
            result->major = 4;
            result->minor = 1;
            result->displayName = "Android 4.1";
            break;
        case 17:
            result->major = 4;
            result->minor = 2;
            result->displayName = "Android 4.2";
            break;
        case 18:
            result->major = 4;
            result->minor = 3;
            result->displayName = "Android 4.3";
            break;
        case 19:
            result->major = 4;
            result->minor = 4;
            result->displayName = "Android 4.4";
            break;
        case 20:
            result->major = 4;
            result->minor = 4;
            result->displayName = "Android Wear 4.4";
            break;
        case 21:
            result->major = 5;
            result->minor = 0;
            result->displayName = "Android 5.0";
            break;
        case 22:
            result->major = 5;
            result->minor = 1;
            result->displayName = "Android 5.1";
            break;
        case 23:
            result->major = 6;
            result->minor = 0;
            result->displayName = "Android 6.0";
            break;
        case 24:
            result->major = 7;
            result->minor = 0;
            result->displayName = "Android 7.0";
            break;
        case 25:
            result->major = 7;
            result->minor = 1;
            result->displayName = "Android 7.1";
            break;
        default:
            result->major = 7;
            result->minor = 1;
            result->displayName = "Android 7.1 or Later";
            break;
    }

    impl->versionCache = *result;

    return true;
}

}

extern "C" {

JNIEXPORT void JNICALL Java_com_eaglesakura_protoground_ProcessContextImpl_newThreadCall(JNIEnv *env, jclass clazz, jlong funcPtr, jlong funcArg) {
    uint32_t lowod = funcPtr & 0xFFFFFFFF;
    uint32_t highword = (funcPtr >> 32);

    ((es::AndroidProcessContext_ThreadCallback) funcPtr)((void *) funcArg);
}

}


#endif /* __ANDROID__ */
