package com.eaglesakura.protoground;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Build;

import java.io.File;

class ProcessContextImpl {

    /**
     * エンディアンチェック用のコードを返却する
     *
     * @return
     */
    static int getEndianCheckCode() {
        return 0x12345678;
    }

    /**
     * @return
     */
    static int getSdkVersion() {
        return Build.VERSION.SDK_INT;
    }

    /**
     * Android Packageがデバッグとしてビルドされている場合true
     *
     * @param context
     * @return
     */
    static boolean isAndroidDebugable(Context context) {
        PackageManager manager = context.getPackageManager();
        ApplicationInfo appInfo = null;
        try {
            appInfo = manager.getApplicationInfo(context.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
        if ((appInfo.flags & ApplicationInfo.FLAG_DEBUGGABLE) == ApplicationInfo.FLAG_DEBUGGABLE) {
            return true;
        }
        return false;
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
    static native void newThreadCall(long functionPtr, long functionArgPtr);
}
