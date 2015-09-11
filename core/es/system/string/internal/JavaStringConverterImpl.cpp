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

std::wstring JavaStringConverterImpl::toWideString(const std::string &utf8) {
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
    std::vector<jbyte> rawBytes((size_t) jConvertedArray_length + sizeof(wchar_t));
    util::zeromemory(&rawBytes);
    // 値をコピーする
    env->GetByteArrayRegion(jConvertedArray, 0, jConvertedArray_length, util::asPointer(rawBytes));

    // temp領域を解放する
    env->DeleteLocalRef(jConvertedArray);
    jConvertedArray = nullptr;

    return std::wstring((wchar_t *) util::asPointer(rawBytes));
}

void JavaStringConverterImpl::initialize() {
    if (UTF32LE.hasObject()) {
        return;
    }

    JNIEnv *env = jc::jni::getThreadJniEnv();

    UTF32LE = string_wrapper(env->NewStringUTF("UTF32LE"), env, false);
    UTF32LE.globalRef().multiThread(true);

    UTF8 = string_wrapper(env->NewStringUTF("UTF32LE"), env, false);
    UTF8.globalRef().multiThread(true);

    // getBytes(String)
    javaString.method_getBytes = env->GetMethodID(UTF32LE.getClass(), "getBytes", "(Ljava/lang/String;)[B");
    assert(javaString.method_getBytes);
}

string JavaStringConverterImpl::toUtf8String(const wide_string &wide) {
    // 実行時の遅延初期化を行う
    initialize();

    JNIEnv *env = jc::jni::getThreadJniEnv();

    // Java文字列に変換する
    string_wrapper temp = string_wrapper(env->NewString((jchar *) wide.c_str(), wide.length()), env, false);

    // getBytesで変換する
    jbyteArray jConvertedArray = (jbyteArray) env->CallObjectMethod(temp.getJobject(), javaString.method_getBytes, UTF8.getJobject());
    assert(jConvertedArray);

    // byte配列から中味を取り出す
    int jConvertedArray_length = env->GetArrayLength(jConvertedArray);
    assert(jConvertedArray_length > 0);
    std::vector<jbyte> rawBytes((size_t) jConvertedArray_length + sizeof(uint8_t));
    util::zeromemory(&rawBytes);
    // 値をコピーする
    env->GetByteArrayRegion(jConvertedArray, 0, jConvertedArray_length, util::asPointer(rawBytes));

    // temp領域を解放する
    env->DeleteLocalRef(jConvertedArray);
    jConvertedArray = nullptr;

    return std::string((char *) util::asPointer(rawBytes));
}
}
}

#endif /* __ANDROID__ */