#pragma once

#include "es/protoground-fbx.hpp"
#include "es/math/protoground-glm.hpp"
#include <vector>
#include <es/graphics/model/file/AnimationData.h>
#include <es/graphics/model/file/SymbolTable.h>

namespace es {
namespace fbx {

class MeshBuilder;

class FbxNodeTree : public virtual Object {
public:
    enum NodeType_e {
        NodeType_Null,
        NodeType_Mesh,
    };

    /**
     * ノードIDを取得する。
     * これはrootノードを含めて0番からの通し番号として機能する。
     */
    uint16_t getNodeId() const { return nodeId; }

    uint64_t getFbxUniqueId() const { return fbx.uid; }

    FbxNode *getFbxNodeRef() const { return fbx.node.get(); }

    /**
     * 全てのノードの合計数を取得する。
     * ノードは連番なので、maxのnodeId +1がノード数となる。
     */
    uint32_t getNodeNum() const;

    /**
     * ノード名を取得する
     */
    const string &getName() const { return name; }

    /**
     * ノードの種類を取得する
     */
    virtual NodeType_e getNodeType() const { return NodeType_Null; }

    /**
     * メッシュの構築を行う。
     * 再帰的に構築し、メッシュは子ノードを含めた全てを含めることになる。
     */
    virtual void buildMesh(MeshBuilder *builder) const;

    /**
     * アニメーション情報を書き出す
     */
    virtual void buildAnimation(file::AnimationData *result, file::SymbolTable *table, const int minFrameIndex = -1, const int maxFrameIndex = -1) const;

    std::shared_ptr<FbxNodeTree> lockParentNode() const;

    std::shared_ptr<FbxNodeTree> lockRootNode() const;

    /**
     * FBXのノードIDからノードを探す
     */
    std::shared_ptr<FbxNodeTree> findNodeFromFbxId(const uint64_t fbxUniqueId) const;

    /**
     * ローカルのノードIDからノードを探す
     */
    std::shared_ptr<FbxNodeTree> findNodeFromId(const uint16_t nodeId) const;

    vec3 getDefaultTranslate() const;

    quat getDefaultRotate() const;

    float getDefaultScale() const;

    virtual ~FbxNodeTree() = default;

    static std::shared_ptr<FbxNodeTree> createNodeTree(std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene);

protected:
    FbxNodeTree();

    class NodeInitializer;

    /**
     * ノードを初期化する
     */
    virtual bool initialize(
            std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene,
            wp<FbxNodeTree> parent, wp<FbxNodeTree> self,
            sp<FbxNode> fbxNode,
            NodeInitializer *initializer
    );

    /**
     * 初期情報を設定する
     */
    virtual void registerDefaultTake();

    wp<FbxNodeTree> self;
    wp<FbxNodeTree> parent;
    std::vector<sp<FbxNodeTree> > children;
    uint16_t nodeId = 0;
    string name;

    struct {
        /**
         * 初期位置
         */
        vec3 position;

        /**
         * 回転角（XYZ順に適用する
         */
        vec3 rotateXYZ;

        /**
         * 初期回転角度
         */
        quat rotate;

        /**
         * スケーリング値
         */
        float scale;
    } transform;

    struct {
        /**
         * FBXのノード本体
         */
        sp<FbxNode> node;

        /**
         * FBXのID
         */
        uint64_t uid = 0;
    } fbx;
};


}
}



