#include <es/asset/file/ChunkFile.h>
#include "ModelData.h"

namespace es {
namespace file {


#if 0
bool ModelData::serialize(std::shared_ptr<IWriter> writer,
                          const std::shared_ptr<SymbolTable> symbols,
                          const MeshData::Serialize *mesh, const MaterialData::Serialize *material,
                          const BoneData::Serialize *bone,
                          const MorphData::Serialize *morph,
                          const PhysicsData::Serialize *physics) {

    class ModelDataWriteCallback : public ChunkFile::WriteCallback {
    public:
        enum {
            DATA_NUM = 6,
        };
        ByteBuffer buffers[DATA_NUM];
        uint32_t uidTable[DATA_NUM];

        /**
         * チャンク数を取得する
         */
        virtual unsigned getChunkNum() {
            return DATA_NUM;
        }

        /**
         * チャンクヘッダを書き込む
         *
         * ChunkHeader::offsetは自動的に計算される
         */
        virtual void writeChunkHeader(const unsigned chunkIndex, ChunkFile::ChunkHeader *header) {

            header->uid = uidTable[chunkIndex];
            header->bytes = buffers->length();
        }

        /**
         * データ本体を書き込む
         */
        virtual bool writeChunkData(const unsigned chunkIndex, ChunkFile::ChunkHeader *header, std::shared_ptr<IWriter> writer) {
            if (buffers[chunkIndex].empty()) {
                return true;
            }
            writer->write(buffers[0].get(), buffers[0].length());
        }
    } callback;

    unsigned index = 0;
    // シンボルテーブル
    {
        callback.buffers[index] = symbols->serialize();

        ++index;
    }

    callback.mesh = mesh;
    callback.bone = bone;
    callback.material = material;
    callback.morph = morph;
    callback.physics = physics;
    callback.symbolBuffer = symbols->serialize();

    return ChunkFile::write(writer, &callback);
}
#endif

}
}