#include "FbxFileConverter.h"
#include <es/asset/IAsset.hpp>
#include <es/fbx/internal/mesh/FbxNodeTree.h>
#include "es/fbx/internal/stream/FbxInputStream.hpp"
#include "es/internal/protoground-internal.hpp"
#include "es/fbx/internal/mesh/MeshBuilder.h"

namespace es {
namespace fbx {


FbxFileConverter::FbxFileConverter() {
}


FbxFileConverter::~FbxFileConverter() {
    fbxScene.reset();
    fbxImportManager.reset();
    fbxManager.reset();
}

bool FbxFileConverter::initialize(file::SymbolTable *resultSymbolTable, std::shared_ptr<IAsset> fbx) {
    // データを読み込む
    // このデータはSDK内部でアクセスが継続されるため、importerよりも生存期間が長くなければならない
    this->buffer = util::toByteArray(fbx);
    assert(buffer.get());
    this->stream.reset(new internal::FbxInputStream(buffer.length()));

    file::SymbolTable dummyTable;
    if (!resultSymbolTable) {
        resultSymbolTable = &dummyTable;
    }

    // FBX初期化
    fbxManager = wrapFbxSharedPtr(FbxManager::Create());
    fbxImportManager = wrapFbxSharedPtr(FbxImporter::Create(fbxManager.get(), nullptr));

    // データを初期化する
    if (!fbxImportManager->Initialize(stream.get(), buffer.get())) {
        eslog("FbxImportManager::Initialize error");
        assert(false);
        return false;
    }
    if (!fbxImportManager->IsFBX()) {
        eslog("FbxImportManager::IsFBX()");
        assert(false);
        return false;
    }

    // シーンを読み込む
    fbxScene = wrapFbxSharedPtr(FbxScene::Create(fbxManager.get(), ""));
    assert(fbxScene);

    if (!fbxImportManager->Import(fbxScene.get())) {
        eslog("FbxImportManager::Import()");
        assert(false);
        return false;
    }

    assert(fbxScene->GetRootNode());
//    // 三角ポリゴン変換
//    {
//        FbxGeometryConverter cvt(fbxManager.get());
//        cvt.Triangulate(fbxScene.get(), true);
//    }

    // ノードを生成
    nodeTree = FbxNodeTree::createNodeTree(fbxImportManager, fbxScene);
    if (!nodeTree) {
        eslog("FbxNodeTree convert failed");
        assert(false);
        return false;
    }

    return true;
}

void FbxFileConverter::convert(file::SkinMeshModelData *resultSkin, file::SymbolTable *resultSymbolTable) {
    assert(resultSkin);

    file::SymbolTable dummyTable;
    if (!resultSymbolTable) {
        resultSymbolTable = &dummyTable;
    }

    memset(&resultSkin->bone.meta, 0x00, sizeof(resultSkin->bone.meta));
    memset(&resultSkin->material.meta, 0x00, sizeof(resultSkin->material.meta));
    memset(&resultSkin->mesh.meta, 0x00, sizeof(resultSkin->mesh.meta));
    memset(&resultSkin->morph.meta, 0x00, sizeof(resultSkin->morph.meta));
    memset(&resultSkin->physics.meta, 0x00, sizeof(resultSkin->physics.meta));

    // メッシュを構築する
    MeshBuilder builder;
    nodeTree->buildMesh(&builder);
    // メッシュに変換する
    builder.computeMesh(resultSkin, resultSymbolTable);
    builder.computeBones(nodeTree, resultSkin, resultSymbolTable);

    eslog("FbxBuilded Mesh Vertices(%d) Materials(%d)", builder.getVertexCount(), builder.getMaterialCount());
}

void FbxFileConverter::convert(file::AnimationData *resultAnim, file::SymbolTable *resultSymbolTable, const int minFrameIndex, const int maxFrameIndex) {
    assert(resultAnim);

    file::SymbolTable dummyTable;
    if (!resultSymbolTable) {
        resultSymbolTable = &dummyTable;
    }

    // アニメーションを構築する
    memset(&resultAnim->animation.meta, 0x00, sizeof(file::AnimationData::Meta));
    resultAnim->animation.meta.flags |= file::AnimationData::Meta::Flag_BoneHasParentOffset;
    nodeTree->buildAnimation(resultAnim, resultSymbolTable, minFrameIndex, maxFrameIndex);

    auto &animation = resultAnim->animation;
    animation.meta.boneKeyNum = animation.boneKeys.size();
    animation.meta.linkBoneNum = animation.linkBones.size();
}

}
}