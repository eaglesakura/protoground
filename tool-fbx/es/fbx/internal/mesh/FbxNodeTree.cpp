#include "FbxNodeTree.h"
#include "FbxMeshNode.h"
#include <es/internal/log/Log.h>
#include <es/math/VectorMath.hpp>

namespace es {
namespace fbx {

class FbxNodeTree::NodeInitializer {
public:
    NodeInitializer() = default;

    uint16_t nextNodeId() {
        return nodeId++;
    }

    ~NodeInitializer() = default;

private:
    uint16_t nodeId = 0;
};

FbxNodeTree::FbxNodeTree() {

}

std::shared_ptr<es::fbx::FbxNodeTree> FbxNodeTree::lockParentNode() const {
    return parent.lock();
}

std::shared_ptr<es::fbx::FbxNodeTree> FbxNodeTree::lockRootNode() const {
    auto parent = lockParentNode();
    if (parent) {
        return parent->lockRootNode();
    } else {
        return self.lock();
    }
}

bool FbxNodeTree::initialize(std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene,
                             wp<FbxNodeTree> parent, wp<FbxNodeTree> self,
                             sp<FbxNode> fbxNode,
                             NodeInitializer *initializer) {
    assert(self.lock().get() == this);

    this->parent = parent;
    this->self = self;
    this->fbx.node = fbxNode;
    this->fbx.uid = fbxNode->GetUniqueID();

    this->name = fbxNode->GetName();
    this->nodeId = initializer->nextNodeId();
    eslog("Node[%03d] Fbx[%03llu] name(%s)", getNodeId(), getFbxUniqueId(), getName().c_str());

    registerDefaultTake();

    for (int i = 0; i < fbxNode->GetChildCount(); ++i) {
        auto child = fbxNode->GetChild(i);

        sp<FbxNodeTree> childNode;
        if (child->GetNodeAttribute()) {
            // メッシュ構造は特殊化する
            if (child->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
                childNode.reset(new FbxMeshNode());
            }
        }

        if (!childNode) {
            childNode.reset(new FbxNodeTree());
        }

        // 子を初期化する
        if (!childNode->initialize(importer, scene, self, childNode, util::wrapSharedPtr(child), initializer)) {
            // 子の初期化失敗
            return false;
        }

        // 子を追加する
        children.push_back(childNode);
    }
    children.shrink_to_fit();

    return true;
}

std::shared_ptr<FbxNodeTree> FbxNodeTree::createNodeTree(std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene) {
    assert(importer);
    assert(scene && scene->GetRootNode());

    NodeInitializer initializer;
    sp<FbxNodeTree> result(new FbxNodeTree());
    if (!result->initialize(importer, scene, std::weak_ptr<FbxNodeTree>(), result, util::wrapSharedPtr(scene->GetRootNode()), &initializer)) {
        return sp<FbxNodeTree>();
    } else {
        return result;
    }
}

void FbxNodeTree::registerDefaultTake() {
    // グローバル行列を取得し、それを要素分解する
    vec3 tempScale;
    util::getTransform(&transform.position, &transform.rotate, &transform.rotateXYZ, &tempScale, fbx.node->EvaluateGlobalTransform());
    transform.scale = (tempScale.x + tempScale.y + tempScale.y) / 3.0f;
    eslog("    type[%d] pos(%.3f, %.3f, %.3f) rot(%.3f, %.3f, %.3f, %.3f) scale(%.3f)",
          getNodeType(),
          transform.position.x, transform.position.y, transform.position.z,
          transform.rotate.x, transform.rotate.y, transform.rotate.z, transform.rotate.w,
          transform.scale
    );
}

void FbxNodeTree::buildMesh(MeshBuilder *builder) const {
    for (auto child : children) {
        child->buildMesh(builder);
    }
}

std::shared_ptr<FbxNodeTree> FbxNodeTree::findNodeFromFbxId(const uint64_t fbxUniqueId) const {
    if (fbx.uid == fbxUniqueId) {
        return self.lock();
    } else {
        for (const auto &child :children) {
            auto node = child->findNodeFromFbxId(fbxUniqueId);
            // 子のどこからか見つかったらそれを返す
            if (node) {
                return node;
            }
        }
    }
    // 見つからなかった
    return sp<FbxNodeTree>();
}

std::shared_ptr<FbxNodeTree> FbxNodeTree::findNodeFromId(const uint16_t nodeId) const {
    if (getNodeId() == nodeId) {
        return self.lock();
    } else {
        for (const auto &child :children) {
            auto node = child->findNodeFromId(nodeId);
            // 子のどこからか見つかったらそれを返す
            if (node) {
                return node;
            }
        }
    }
    // 見つからないなら別なノードツリーに所属している
    return sp<FbxNodeTree>();
}

uint32_t FbxNodeTree::getNodeNum() const {

    if (children.empty()) {
        // 終端なので、自分のID+1が正解となる。
        return nodeId + 1;
    } else {
        // 末尾の子が必然的に大きなIDが割り当てられているはずである
        return children[children.size() - 1]->getNodeNum();
    }
}


vec3 FbxNodeTree::getDefaultTranslate() const {
    return transform.position;
}

quat FbxNodeTree::getDefaultRotate() const {
//    return quat(transform.rotateXYZ);
    return quat(transform.rotate);
//    return quat(0, 0, 0, 1);
}

float FbxNodeTree::getDefaultScale() const {
    return transform.scale;
}

void FbxNodeTree::buildAnimation(file::AnimationData *result, file::SymbolTable *table, const int minFrameIndex, const int maxFrameIndex) const {

    FbxScene *scene = fbx.node->GetScene();

    // アニメーション名を取得する
    FbxArray<FbxString *> animationNames;
    scene->FillAnimStackNameArray(animationNames);

    FbxTime period;
    //! 1フレーム単位の時間を取得
    period.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

    if (animationNames.Size() == 0) {
        eslog("animation not found...");
        return;
    }

    eslog("Node[%03d] name(%s) Anim[%s]", getNodeId(), getName().c_str(), animationNames[0]->Buffer());
    FbxTakeInfo *info = scene->GetTakeInfo(animationNames[0]->Buffer());

    if (!info) {
        eslog("    - NotFound...");
        return;
    }

    {
        FbxTime start = info->mLocalTimeSpan.GetStart();
        FbxTime end = info->mLocalTimeSpan.GetStop();

        int startFrame = (int) (start.Get() / period.Get());
        int endFrame = (int) (end.Get() / period.Get());

        if (minFrameIndex > 0) {
            startFrame = minFrameIndex;
        }
        if (maxFrameIndex > 0) {
            endFrame = maxFrameIndex;
        }

        assert(endFrame >= startFrame);

        eslog("    Node(%s) Frame %d -> %d", name.c_str(), startFrame, endFrame);

        auto &anim = result->animation;

        file::AnimationData::SymbolTimeline timeline;
        timeline.symbol = table->add(getName());
        timeline.offset = anim.boneKeys.size();

        int currentFrame = 0;
        int added = 0;
        // 全てのキーを打刻する
        file::AnimationData::BoneKeyFrame before;
        // 最終フレームも含まれるため、ループカウンタはendFrameを打刻しなければならない。
        for (int i = startFrame; i <= endFrame; ++i) {
            vec3 scale(1, 1, 1);
            file::AnimationData::BoneKeyFrame key;
            key.frame = currentFrame;
            key.reserve0 = key.reserve1 = 0;

            util::getTransform(&key.pos, &key.rotate, nullptr, &scale, fbx.node->EvaluateLocalTransform(period * i));
            if (!added || !file::AnimationData::someFrame(before, key)) {
                // 未登録か、前のフレームと同じ状態である場合登録する
                anim.boneKeys.push_back(key);
                before = key;
                ++added;
            }

            ++currentFrame;
//            eslog("    - Frame[%d] tr(%.3f, %.3f, %.3f) rot(%.3f, %.3f, %.3f, %.3f) scale(%.3f)",
//                  key.frame,
//                  key.pos.x, key.pos.y, key.pos.z,
//                  key.rotate.x, key.rotate.y, key.rotate.z, key.rotate.w,
//                  cpu_math::average(scale)
//            );
        }
        timeline.maxFrames = (currentFrame - 1);
        timeline.num = added;
        anim.linkBones.push_back(timeline);
        anim.meta.endFrame = std::max<uint16_t>(timeline.maxFrames, anim.meta.endFrame);
    }
    FbxArrayDelete(animationNames);

    // 子を構築する
    for (const auto &child : children) {
        child->buildAnimation(result, table, minFrameIndex, maxFrameIndex);
    }
}
}
}
