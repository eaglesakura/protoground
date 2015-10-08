package com.eaglesakura.protoground;

import android.content.Context;

import java.io.File;
import java.lang.reflect.Field;

class ProcessContextImpl {

    /**
     * Android Packageがデバッグとしてビルドされている場合true
     *
     * @param context
     * @return
     */
    static boolean isAndroidDebugable(Context context) {
        try {
            Class<?> clazz = Class.forName(context.getPackageName() + ".BuildConfig");
            Field field = clazz.getField("DEBUG");
            return field.getBoolean(clazz);
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * データベースをOpenするためのpathを取得する
     *
     * @param context
     * @param basePath
     * @return
     */
    static String getDatabasePath(Context context, String basePath) {
        File file = context.getDatabasePath(basePath);
        if (file != null) {
            return file.getAbsolutePath();
        } else {
            return null;
        }
    }

    static void newThread(String name, final long functionPtr, final long functionArgPtr) {
        Thread thread = new Thread() {
            @Override
            public void run() {
                newThreadCall(functionPtr, functionArgPtr);
            }
        };
        thread.setName(name);
        thread.start();
    }

    /**
     * Native側の呼び出しを行う
     *
     * @param functionPtr    呼び出し先の関数ポインタvoid(*function)(void* ptr)
     * @param functionArgPtr 呼び出し対象の引数
     */
    static native void newThreadCall(final long functionPtr, final long functionArgPtr);
}
