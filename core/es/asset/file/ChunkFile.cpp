#include <es/math/Math.hpp>
#include "ChunkFile.h"

namespace es {

ChunkFile::FileHeader ChunkFile::FileHeader::create() {
    FileHeader header = {0};
    header.magic = FileHeader::MAGIC;
    header.version = FileHeader::VERSION;
    header.headerSize = sizeof(FileHeader);
    header.chunkHeaderSize = sizeof(ChunkHeader);
    return header;
}

ChunkFile::ChunkHeader ChunkFile::ChunkHeader::create() {
    ChunkHeader header = {0};
    return header;
}

bool ChunkFile::write(std::shared_ptr<IWriter> writer, ChunkFile::WriteCallback *callback) {

    // ヘッダを書き込む
    auto header = FileHeader::create();
    {
        header.chunkNum = callback->getChunkNum();
        if (!writer->write(&header, sizeof(FileHeader))) {
            return false;
        }
    }

    // チャンクごとのヘッダ情報を書き込む
    std::vector<ChunkHeader> headers(header.chunkNum);
    for (int i = 0; i < header.chunkNum; ++i) {
        auto chunk = ChunkHeader::create();
        callback->writeChunkHeader(i, &chunk);

        // チャンクを書き込む
        writer->write(&chunk, sizeof(ChunkHeader));
        headers[i] = chunk;
    }

    // 本体データを書き込む
    for (int i = 0; i < header.chunkNum; ++i) {
        if (!callback->writeChunkData(i, &headers[i], writer)) {
            return false;
        }
    }

    return true;
}

bool ChunkFile::parse(std::shared_ptr<IAsset> asset, ChunkFile::ReadCallback *callback) {
    FileHeader header;
    // ヘッダ解析
    {
        util::read(asset, &header, sizeof(FileHeader));
        if (header.magic != FileHeader::MAGIC) {
            callback->onError(ReadCallback::Error_UnsupportFormat);
            return false;
        }

        // フォーマット自体が対応していない
        if (header.version != FileHeader::VERSION) {
            callback->onError(ReadCallback::Error_UnsupportVersion);
            return false;
        }

        // チャンクが規定容量を満たしていない
        if (header.chunkHeaderSize < sizeof(ChunkHeader)) {
            callback->onError(ReadCallback::Error_UnsupportVersion);
            return false;
        }
    }
    // ファイル本体解析
    std::vector<ChunkHeader> chunkHeaders(header.chunkNum);
    for (int i = 0; i < header.chunkNum; ++i) {
        auto buffer = asset->read(header.chunkHeaderSize);
        if (buffer.length < sizeof(ChunkHeader)) {
            // 規定量の読み込みができない
            callback->onError(ReadCallback::Error_ReadFailed);
            return false;
        } else {
            // 必要量だけチャンクをコピーする
            memcpy((void *) (&chunkHeaders[i]), buffer.ptr, sizeof(ChunkHeader));
        }
    }

    // ファイルを読み込む
    for (int i = 0; i < header.chunkNum; ++i) {
        const auto &chunkHeader = chunkHeaders[i];
        int64_t readRequestSize = chunkHeader.bytes;

        ParseOption option;
        callback->getChunkParseOption(&chunkHeader, &option);

        unsigned onceReadBytes = std::min(chunkHeader.bytes, option.onceReadBytes);
        // 0byteである場合は一括読み込み
        if (!onceReadBytes) {
            onceReadBytes = chunkHeader.bytes;
        }

        while (readRequestSize > 0) {
            auto buffer = asset->read(onceReadBytes);
            if (buffer.length < onceReadBytes) {
                // 読み込み失敗
                callback->onError(ReadCallback::Error_ReadFailed);
                return false;
            } else {
                // 読み込みOK
                callback->onChunkParse(&header, &chunkHeader, chunkHeader.bytes - readRequestSize, onceReadBytes, buffer.ptr);
            }
            // 残りサイズを縮める
            readRequestSize -= buffer.length;
        }
    }

    return true;
}
}