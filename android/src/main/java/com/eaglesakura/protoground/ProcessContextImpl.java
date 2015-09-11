package com.eaglesakura.protoground;

class ProcessContextImpl {

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
