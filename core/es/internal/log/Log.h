#pragma once

#include    <string.h>

namespace es {

namespace internal {

/**
 * __FILE__マクロからファイル名を取得する
 */
const char *pathToFileName(const char *__file__);

enum LogType_e {
    /* 通常出力 */
            LogType_Info,

    /* デバッグ中のみ表示したい */
            LogType_Debug,

    /* 特に重要なログで表示する */
            LogType_Alert,
};

class Logger {
public:
    /**
     * libMikuMikuSimple用のログ関数
     * デフォルトログ関数を利用しない場合、適宜書き換える。
     */
    typedef void (*LogFunctionPtr)(const LogType_e type, const char *, const char *, ...);

    /**
     * ログの実装を変更する
     */
    static void setFunction(LogFunctionPtr ptr) {
        Logger::func = ptr;
    }

    /**
     * ログの実装を取得する
     */
    static LogFunctionPtr get() {
        return func;
    }
private:
    static LogFunctionPtr func;
};

}

#if !defined(EGLIBRARY_NO_LOG)

/**
 * 特定条件下の設定
 */
#define eslog_from(file, line, ...)       { ::es::internal::Logger::get()(::es::internal::LogType_Debug, ::es::internal::pathToFileName(file),  "L " es_num_to_str(__LINE__) " | " __VA_ARGS__); }

/**
 * 文字列化用ダミー
 */
#define _dummy_es_num_to_str(num) #num

/**
 * 文字列化する
 */
#define es_num_to_str(num) _dummy_es_num_to_str(num)

/**
 * フォーマット付きログ
 */
#define eslog(...)       ::es::internal::Logger::get()(::es::internal::LogType_Info, ::es::internal::pathToFileName(__FILE__), "L " es_num_to_str(__LINE__) " | " __VA_ARGS__)

/**
 * フォーマット付きログ
 */
#define esdebug(...)     ::es::internal::Logger::get()(::es::internal::LogType_Debug, ::es::internal::pathToFileName(__FILE__), "L " es_num_to_str(__LINE__) " | " __VA_ARGS__)

/**
 * アラート表示
 */
#define esalert(...)      ::es::internal::Logger::get()(::es::internal::LogType_Alert, ::es::internal::pathToFileName(__FILE__), "L %d | " __VA_ARGS__)

#else

/**
 * 特定条件下の設定
 */
#define eslog_from(...)       {  }

/**
 * 単純ログ
 */
#define eslog( ... )            {  }

/**
 * デバッグログ
 */
#define esdebug(... )   {   }

/**
 * アラート表示
 */
#define esalert( ... )      {  }

#endif

}
