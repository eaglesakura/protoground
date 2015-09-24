#ifdef __ANDROID__

#include "JavaStringConverterImpl.h"
#include "es/memory/Buffer.hpp"

using namespace jc;
using namespace jc::lang;

namespace es {
namespace internal {

JavaStringConverterImpl::JavaStringConverterImpl() : UTF32LE(string_wrapper((jobject) NULL)), UTF8(string_wrapper((jobject) NULL)) {

}

JavaStringConverterImpl::~JavaStringConverterImpl() {

}

wide_string JavaStringConverterImpl::toWideString(const std::string &utf8) {
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

void JavaStringConverterImpl::initialize() {
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

string JavaStringConverterImpl::toUtf8String(const wide_string &wide) {
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
}
}

#endif /* __ANDROID__ */