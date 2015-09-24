package com.eaglesakura.protoground;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

class StringImpl {
    static final Charset UTF32LE;

    /**
     * 必要最小限のバッファを確保しておき、頻繁なGCを抑制する
     */
    static final byte[] utf32ConvertBuffer = new byte[1024 * 8];

    static {
        UTF32LE = Charset.forName("UTF32LE");
    }

    static synchronized String UTF32toUTF8(ByteBuffer buffer) {
        byte[] jBuffer;
        if (buffer.capacity() < utf32ConvertBuffer.length) {
            jBuffer = utf32ConvertBuffer;
        } else {
            jBuffer = new byte[buffer.capacity()];
        }
        buffer.get(jBuffer, 0, buffer.capacity());
        try {
            return new String(jBuffer, 0, buffer.capacity(), UTF32LE);
        } catch (Exception e) {
            return "CONVERT_ERROR";
        }
    }
}
