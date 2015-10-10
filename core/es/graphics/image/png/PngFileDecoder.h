#pragma once

#include "es/protoground.hpp"
#include <es/graphics/PixelFormat.hpp>
#include "es/asset/IAsset.hpp"
#include "es/memory/Buffer.hpp"

namespace es {

class IImageDecodeCallback;

/**
 * PNGファイルのデコードを行う。
 *
 * libpngに依存するため、ビルドはオプションとなる。
 */
class PngFileDecoder {
public:
    PngFileDecoder();

    ~PngFileDecoder();

    /**
     * ファイルフォーマットチェック等、事前に読み込んで閉まっているバッファがあったらそれを渡す
     * 内部ではデータをコピーするため、バッファは解放しても構わない。
     */
    void setReadedBuffer(const unsafe_array<uint8_t> &buffer);

    /**
     * NPOT -> POT変換を行う場合はtrue
     */
    void setConvertPot(bool set);

    /**
     * 一度の読み込みで読み込む行数（高さ）を指定する。
     *
     * 0を指定した場合、全データを一括で読み込む。
     * デフォルトは0になっている。
     */
    void setOnceReadHeight(const unsigned heightPixels);

    /**
     * ピクセルフォーマットの変換リクエストを行う。
     */
    void setConvertPixelFormat(const PixelFormat_e format);

    /**
     * アセットをPNGファイルとして読み込む
     *
     * 読み込みに成功したらtrue、それ以外はfalseを返却する。
     */
    bool load(std::shared_ptr<IAsset> asset, selection_ptr<IImageDecodeCallback> listener);

    /**
     * ファイル先頭をチェックし、PNGファイルであればtrueを返す。
     */
    static bool isPngFile(const unsafe_array<uint8_t> &sign);

private:
    ByteBuffer readedBuffer;

    /**
     * 一度に読み込む高さピクセル数。
     * 小さくするほど省メモリになるが、オーバーヘッドが大きくなる。
     * 0の場合一括で全てを読み込む。
     */
    uint16_t onceReadLines = 0;

    /**
     * NPOT変換を行う場合true
     */
    bool convertPot = false;

    /**
     * ピクセルフォーマット変換をリクエストする場合、指定のフォーマットを設定する。
     *
     * ただし、未サポートのピクセルフォーマットを指定しようとするとfalseを返却してロードを強制終了する。
     */
    PixelFormat_e pixelConvert = PixelFormat_RGBA8888;
};

}
