#pragma once

#include "es/protoground-fbx.hpp"
#include "FbxNodeTree.h"
#include "VertexContainer.h"
#include "PolygonContainer.h"

namespace es {
namespace fbx {


class FbxMeshNode : public FbxNodeTree {
public:
    virtual NodeType_e getNodeType() const override;

    virtual void buildMesh(MeshBuilder *builder) const override;

protected:
    virtual bool initialize(std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene,
                            wp<FbxNodeTree> parent, wp<FbxNodeTree> self, sp<FbxNode> fbxNode,
                            NodeInitializer *initializer) override;

    VertexContainer vertices;
    PolygonContainer polygons;
};


}
}
