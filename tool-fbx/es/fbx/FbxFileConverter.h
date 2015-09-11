#pragma once

#include <es/fbx/internal/mesh/FbxNodeTree.h>
#include "es/protoground-fbx.hpp"
#include "es/graphics/model/file/ModelData.h"
#include "es/graphics/model/file/AnimationData.h"

namespace es {
namespace fbx {

class FbxFileConverter {
public:
    FbxFileConverter();

    /**
     * 初期化する
     */
    bool initialize(file::SymbolTable *resultSymbolTable, std::shared_ptr<IAsset> fbx);

    /**
     * メッシュを変換する
     */
    void convert(file::SkinMeshModelData *resultSkin, file::SymbolTable *resultSymbolTable);

    /**
     * アニメーションを変換する
     *
     * 指定範囲を取り出したい場合はmin〜maxのフレームインデックスを指定する。
     */
    void convert(file::AnimationData *resultAnim, file::SymbolTable *resultSymbolTable, const int minFrameIndex = -1, const int maxFrameIndex = -1);

    ~FbxFileConverter();

private:
    ByteBuffer buffer;
    sp<FbxStream> stream;
    sp<FbxManager> fbxManager;
    sp<FbxImporter> fbxImportManager;
    sp<FbxScene> fbxScene;
    sp<FbxNodeTree> nodeTree;
};

}
}


