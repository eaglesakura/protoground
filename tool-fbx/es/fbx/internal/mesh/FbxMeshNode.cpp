#include "FbxMeshNode.h"
#include "VertexContainer.h"
#include "PolygonContainer.h"
#include <es/internal/log/Log.h>
#include "MeshBuilder.h"

namespace es {
namespace fbx {

FbxNodeTree::NodeType_e FbxMeshNode::getNodeType() const {
    return NodeType_Mesh;
}

bool FbxMeshNode::initialize(std::shared_ptr<FbxImporter> importer, std::shared_ptr<FbxScene> scene,
                             wp<FbxNodeTree> parent, wp<FbxNodeTree> self, sp<FbxNode> fbxNode,
                             FbxNodeTree::NodeInitializer *initializer) {
    if (!FbxNodeTree::initialize(importer, scene, parent, self, fbxNode, initializer)) {
        return false;
    }

    assert(fbxNode->GetNodeAttribute() && fbxNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh);
    assert(fbxNode->GetMesh());
#if 0
    {
        const char* debugNodeName = fbxNode->GetName();
        FbxGeometry *geometry = ((FbxGeometry*) fbxNode->GetNodeAttribute());
        const unsigned deformers = geometry->GetDeformerCount(FbxDeformer::eSkin);
        unsigned clusterCount = 0;
        for (unsigned i = 0; i < deformers; ++i) {
            FbxSkin *skin = (FbxSkin*) (geometry->GetDeformer(i, FbxDeformer::eSkin));
            clusterCount += skin->GetClusterCount();

            for (int k = 0; k < skin->GetClusterCount(); ++k) {
                FbxCluster *cluster = skin->GetCluster(k);
                const char *culsterName = cluster->GetLink()->GetName();

                const unsigned pointsNum = cluster->GetControlPointIndicesCount();
                const int* indicesRef = cluster->GetControlPointIndices();
                const double* weightsRef = cluster->GetControlPointWeights();

                eslog("    - Debug :: Cluster[%02d] id[%llu] link[%llu] name(%s) geoms(%d)",
                      k, cluster->GetUniqueID(), cluster->GetLink()->GetUniqueID(),
                      culsterName,
                      pointsNum
                      );
            }
        }

        eslog("Node [%s] Clusters[%d]", debugNodeName, clusterCount);
    }
#endif

    auto mesh = fbxNode->GetMesh();

    vertices.build(mesh);
    polygons.build(mesh);

    assert(vertices.valid());
    return true;
}

void FbxMeshNode::buildMesh(MeshBuilder *builder) const {
    // クラスタから初期姿勢を再制御する
    if (fbx.node->GetMesh()) {
        FbxMesh *mesh = fbx.node->GetMesh();

        auto rootNode = lockRootNode();

        for (int i = 0; i < mesh->GetDeformerCount(); ++i) {
            FbxSkin *skin = (FbxSkin *) mesh->GetDeformer(i, FbxDeformer::eSkin);
            for (int k = 0; k < skin->GetClusterCount(); ++k) {
                FbxCluster *cluster = skin->GetCluster(k);

                auto  linkNode = rootNode->findNodeFromFbxId(cluster->GetLink()->GetUniqueID());
                assert(linkNode);

                linkNode->registerDefaultTake(cluster);
            }
        }
    }

    // 自身を構築する
    builder->add(lockRootNode(), this, &vertices, &polygons);

    FbxNodeTree::buildMesh(builder);
}
}
}