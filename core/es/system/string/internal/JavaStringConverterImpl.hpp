#ifdef __ANDROID__
#pragma once

#include "es/system/string/IStringConverter.h"
#include <jni.h>
#include "JointConnector.hpp"
#include "es/memory/Buffer.hpp"

using namespace jc;
using namespace jc::lang;

namespace es {
namespace internal {

/**
 * JavaのString.getBytesを経由してUTF8->UTF32LE変換を行う
 */
class JavaStringConverterImpl : public IStringConverter {
public:
    JavaStringConverterImpl() : UTF32LE(string_wrapper((jobject) NULL)), UTF8(string_wrapper((jobject) NULL)) {
    }

    virtual ~JavaStringConverterImpl() = default;

    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual wide_string toWideString(const string &utf8) override {
        if (utf8.empty()) {
            return wide_string();
        }

        // 実行時の遅延初期化を行う
        initialize();

        JNIEnv *env = jc::jni::getThreadJniEnv();

        // Java文字列に変換する
        string_wrapper temp = string_wrapper(env->NewStringUTF(utf8.c_str()), env, false);

        // getBytesで変換する
        jbyteArray jConvertedArray = (jbyteArray) env->CallObjectMethod(temp.getJobject(), javaString.method_getBytes, UTF32LE.getJobject());
        assert(jConvertedArray);

        // byte配列から中味を取り出す
        int jConvertedArray_length = env->GetArrayLength(jConvertedArray);
        assert(jConvertedArray_length > 0);
        std::vector<jbyte> rawBytes((size_t) jConvertedArray_length + sizeof(wide_char));
        util::zeromemory(&rawBytes);
        // 値をコピーする
        env->GetByteArrayRegion(jConvertedArray, 0, jConvertedArray_length, util::asPointer(rawBytes));

        // temp領域を解放する
        env->DeleteLocalRef(jConvertedArray);
        jConvertedArray = nullptr;

        wide_char *msg = (wide_char *) util::asPointer(rawBytes);
        return wide_string(msg);
    }


    virtual string toUtf8String(const wide_string &wide) override {
        if (wide.empty()) {
            return string();
        }

        // 実行時の遅延初期化を行う
        initialize();

        JNIEnv *env = jc::jni::getThreadJniEnv();

        // Java文字列に変換する
        const wide_char *src = wide.c_str();
        const uint32_t src_memLength = sizeof(wide_char) * wide.length();

        jobject jUTF32Buffer = env->NewDirectByteBuffer((void *) src, src_memLength);
        jobject jUTF8String = env->CallStaticObjectMethod(
                stringImpl.clazz.getJclass(),
                stringImpl.method_UTF32toUTF8,
                jUTF32Buffer
        );

        // バッファを削除する
        env->DeleteLocalRef(jUTF32Buffer);

        if (jUTF8String) {
            // string convert
            jc::lang::string_wrapper str(jUTF8String, env, false);
            string result = str.asString();
            return result;
        } else {
            return string();
        }
    }

private:
    struct {
        jmethodID method_getBytes = nullptr;
    } javaString;

    struct {
        jc::lang::class_wrapper clazz;

        jmethodID method_UTF32toUTF8;
    } stringImpl;

    jc::lang::string_wrapper UTF32LE;
    jc::lang::string_wrapper UTF8;

    void initialize() {
        if (stringImpl.clazz.hasObject()) {
            return;
        }

        JNIEnv *env = jc::jni::getThreadJniEnv();

        {
            stringImpl.clazz = class_wrapper::find(env, "com/eaglesakura/protoground/StringImpl");
            assert(stringImpl.clazz.hasObject());
            stringImpl.clazz.globalRef().multiThread(true);

            stringImpl.method_UTF32toUTF8 = stringImpl.clazz.getMethod("UTF32toUTF8", "(Ljava/nio/ByteBuffer;)Ljava/lang/String;", true);
            assert(stringImpl.method_UTF32toUTF8);
        }

        UTF32LE = string_wrapper(env->NewStringUTF("UTF32LE"), env, false);
        UTF32LE.globalRef().multiThread(true);

        UTF8 = string_wrapper(env->NewStringUTF("UTF32LE"), env, false);
        UTF8.globalRef().multiThread(true);

        // getBytes(String)
        javaString.method_getBytes = env->GetMethodID(UTF32LE.getClass(), "getBytes", "(Ljava/lang/String;)[B");
        assert(javaString.method_getBytes);
    }
};

}
}

#endif /* __ANDROID__ */