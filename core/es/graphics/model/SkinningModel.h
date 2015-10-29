#pragma once

#include <es/graphics/model/file/SymbolTable.h>
#include <es/graphics/model/VertexAttribute.h>
#include "es/protoground.hpp"
#include "es/graphics/resource/IMesh.hpp"
#include "es/graphics/model/pose/AnimationPose.h"
#include "es/graphics/model/pose/BoneController.h"

namespace es {

namespace file {
struct SkinMeshModelData;
}

class AnimationPose;

/**
 * 共通モデルデータを管理する
 */
class SkinningModel : public Object {
public:

    SkinningModel();

    /**
     * 空のメッシュオブジェクトに対してメモリ確保を行う。
     *
     * 初期データのアップロードは行わないため、別途uploadを行う必要がある。
     */
    virtual void initialize(
            const std::shared_ptr<IDevice> device,
            const std::shared_ptr<IMesh> emptyMesh, const uint32_t verticesAllocFlags, const uint32_t indicesAllocFlags,
            const std::shared_ptr<file::SkinMeshModelData> model,
            const std::shared_ptr<file::SymbolTable> symbols = std::shared_ptr<es::file::SymbolTable>()
    );

    /**
     * 不要になったデータを解放する
     */
    void release(const uint32_t releaseFlags);

    /**
     * 指定された属性をModelからメッシュにコピーする。
     *
     * attributesがnullptrの場合、全てのデータを丸ごとコピーする。
     */
    void uploadVertices(std::shared_ptr<IDevice> device, const VertexAttribute *attributes);

    /**
     * インデックスデータをメッシュにコピーする。
     */
    void uploadIndices(std::shared_ptr<IDevice> device);

    const std::shared_ptr<IMesh> &getMesh() const;

    const std::shared_ptr<file::SkinMeshModelData> &getModel() const;

    const std::shared_ptr<file::SymbolTable> &getSymbols() const;

    const std::string getSymbolString(const uint64_t symbol) const;

    const std::shared_ptr<BoneController> &getBoneController() const;

    const VertexAttribute::Complex getVertexAttributes() const;

    /**
     * 新しいアニメーション用メモリを生成する。
     */
    virtual std::shared_ptr<AnimationPose> newAnimationPose() const;

    /**
     * モデルがボーン構造を持っていればtrue
     */
    bool hasBones() const;

    /**
     * モデルが物理構造を持っていればtrue
     */
    bool hasPhysics() const;

    virtual ~SkinningModel() = default;

    enum ReleaseFlag_e {
        /**
         * 頂点データを解放する
         */
                ReleaseFlag_Vertices = 0x1 << 0,

        /**
         * インデックスを解放する
         */
                ReleaseFlag_Indices = 0x1 << 1,


        /**
         * シンボルテーブルを解放する
         */
                ReleaseFlag_Symbols = 0x1 << 2,
    };
private:
    sp<IMesh> mesh;
    VertexAttribute::Complex attribute;

    sp<file::SkinMeshModelData> model;
    sp<file::SymbolTable> symbols;

    /**
     * ボーン処理用データ
     */
    sp<BoneController> boneController;

};


}

