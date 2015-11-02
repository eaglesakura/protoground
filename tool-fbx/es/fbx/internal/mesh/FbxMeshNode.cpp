#include "FbxMeshNode.h"
#include "VertexContainer.h"
#include "PolygonContainer.h"
#include <es/internal/log/Log.h>
#include "MeshBuilder.h"

namespace es {
namespace fbx {

FbxNodeTree::NodeType_e FbxMeshNode::getNodeType() const {
    return FbxNodeTree::getNodeType();
}

bool FbxMeshNode::initialize(std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene,
                             wp<FbxNodeTree> parent, wp<FbxNodeTree> self, sp<FbxNode> fbxNode,
                             FbxNodeTree::NodeInitializer *initializer) {
    if (!FbxNodeTree::initialize(importer, scene, parent, self, fbxNode, initializer)) {
        return false;
    }

    assert(fbxNode->GetNodeAttribute() && fbxNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh);
    assert(fbxNode->GetMesh());

    auto mesh = fbxNode->GetMesh();

    vertices.build(mesh);
    polygons.build(mesh);

    assert(vertices.valid());
    return true;
}

void FbxMeshNode::buildMesh(MeshBuilder *builder) const {
    // 自身を構築する
    builder->add(lockRootNode(), this, &vertices, &polygons);

    FbxNodeTree::buildMesh(builder);
}
}
}