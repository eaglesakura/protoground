#pragma once

#include "es/protoground.hpp"
#include <es/asset/IAsset.hpp>
#include <functional>
#include <es/asset/IWriter.hpp>

namespace es {

/**
 * protogroundで対応する独自形式を扱うためのフォーマット
 */
class ChunkFile {
public:
    /**
     * ファイルの先頭に必ず付属するヘッダ
     */
    struct FileHeader {
        enum {
            MAGIC = 0x00310300,
            VERSION = 0x1,
        };
        /**
         * 識別用のマジックナンバー
         */
        uint32_t magic;

        /**
         * ファイルバージョン
         */
        uint16_t version;
        /**
         * チャンクのヘッダ要素の開始位置
         */
        uint32_t headerSize;

        /**
         * チャンクヘッダのサイズ
         */
        uint32_t chunkHeaderSize;

        /**
         * 含まれているチャンク数
         */
        uint16_t chunkNum;

        /**
         * デフォルトのヘッダを生成する
         */
        static FileHeader create();
    };

    /**
     * チャンク情報
     *
     * チャンク自体のフォーマットには関与しない
     */
    struct ChunkHeader {
        /**
         * チャンクの識別ID
         */
        uint32_t uid;

        /**
         * チャンクの大きさ（バイト数）
         */
        uint32_t bytes;

        /**
         * UIDごとに自由に指定可能なデータ領域
         */
        uint32_t user;

        static ChunkHeader create();
    };

    class WriteCallback {
    public:
        /**
         * チャンク数を取得する
         */
        virtual uint getChunkNum() = 0;

        /**
         * チャンクヘッダを書き込む
         *
         * ChunkHeader::offsetは自動的に計算される
         */
        virtual void writeChunkHeader(const uint chunkIndex, ChunkHeader *header) = 0;

        /**
         * データ本体を書き込む
         */
        virtual bool writeChunkData(const uint chunkIndex, ChunkHeader *header, std::shared_ptr<IWriter> writer) = 0;

        virtual ~WriteCallback() = default;
    };

    struct ParseOption {
        /**
         * 一度の読み込みで渡されるサイズ
         * 巨大なファイルを読み込む場合に、分割して読み込めるようにする
         */
        uint32_t onceReadBytes = 0;

        ParseOption() = default;
    };

    class ReadCallback {
    public:
        enum Error_e {
            /**
             * 非対応フォーマット
             */
                    Error_UnsupportFormat,

            /**
             * バージョンエラー
             */
                    Error_UnsupportVersion,

            /**
             * 読み込みエラー
             */
                    Error_ReadFailed,
        };

        /**
         * ヘッダをパースした
         */
        virtual void onHeaderParse(const FileHeader *header) = 0;

        /**
         * チャンクの読み込みオプションを取得する
         */
        virtual void getChunkParseOption(const ChunkHeader *header, ParseOption *option) = 0;

        /**
         * チャンクをパースした
         *
         * @param header ファイルヘッダ
         * @param chunkHeader チャンクヘッダ
         * @param offset チャンクデータの頭からのオフセット量 分割読み込みが行われた場合は0以外が渡される
         * @param size データサイズ
         * @param data データ本体
         */
        virtual void onChunkParse(const FileHeader *header, const ChunkHeader *chunkHeader, const uint offset, const uint size, const void *data) = 0;

        /**
         * エラーが発生した
         */
        virtual void onError(Error_e error) = 0;
    };

    /**
     * チャンク情報の書き込みを行う
     */
    static bool write(std::shared_ptr<IWriter> writer, WriteCallback *callback);

    /**
     * チャンク情報の読み込みを行う
     */
    static bool parse(std::shared_ptr<IAsset> asset, ReadCallback *callback);

    enum {
        UID_BASE = 0x00100000,

        /**
         * シンボルテーブル
         */
                UID_GRAPHICS_MODEL_SYMBOLS,

        /**
         * メッシュテーブル
         */
                UID_GRAPHICS_MODEL_MESH,

        /**
         * マテリアル情報
         */
                UID_GRAPHICS_MODEL_MATERIALS,

        /**
         * モーフィング情報
         */
                UID_GRAPHICS_MODEL_MORPHS,

        /**
         * 物理情報
         */
                UID_GRAPHICS_MODEL_PHYSICS,
    };
};

}

