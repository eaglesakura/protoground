#include "VertexContainer.h"
#include <es/memory/SafeArray.hpp>
#include <es/internal/log/Log.h>

namespace es {
namespace fbx {

void VertexContainer::build(FbxMesh *mesh) {
    createPositions(mesh);
    createNormals(mesh);
    createColors(mesh);
    createCoords(mesh);

    eslog("    - Mesh pos(%d) attr(%d)", positions.size(), normals.size());

    createWeights(mesh);

    assert(valid());
}

void VertexContainer::createPositions(FbxMesh *mesh) {
    const int ctrlNum = mesh->GetControlPointsCount();
    FbxVector4 *points = mesh->GetControlPoints();

    // 頂点をコピーする
    util::valloc(&positions, ctrlNum, false);
    for (int i = 0; i < ctrlNum; ++i) {
        positions[i] = vec3(points[i][0], points[i][1], points[i][2]);
    }

}

void VertexContainer::createNormals(FbxMesh *mesh) {
    const int layerNum = mesh->GetElementNormalCount();

    for (int i = 0; i < layerNum; ++i) {
        const FbxLayerElementNormal *normals = mesh->GetElementNormal(i);
        //! normal
        if (normals && normals->GetReferenceMode() == FbxLayerElement::eDirect) {
            const int normalNum = normals->GetDirectArray().GetCount();

            util::valloc(&this->normals, normalNum, false);

            for (int k = 0; k < normalNum; ++k) {
                FbxVector4 v = normals->GetDirectArray().GetAt(k);
                VertexContainer::normals[k] = vec3(v[0], v[1], v[2]);
            }
            return;
        }
    }

    assert(false);
}


void VertexContainer::createColors(FbxMesh *mesh) {
    const int layerNum = mesh->GetElementVertexColorCount();


    // 頂点カラーを含まない場合、全て白で生成する
    if (!layerNum && !normals.empty()) {
        util::valloc(&colors, normals.size(), false);
        memset(util::asPointer(colors), 0xFF, sizeof(es::Color) * colors.size());
        return;
    }

    for (int i = 0; i < layerNum; ++i) {
        const FbxLayerElementVertexColor *col = mesh->GetElementVertexColor(i);
        if (col && col->GetReferenceMode() == FbxLayerElement::eDirect) {
            const int colorNum = col->GetDirectArray().GetCount();
            util::valloc(&this->colors, colorNum, false);
            for (int k = 0; k < colorNum; ++k) {
                auto v = col->GetDirectArray().GetAt(k);
                VertexContainer::colors[k] = Color::fromRGBAf(v[0], v[1], v[2], v[3]);
            }
            return;
        } else if (col && col->GetReferenceMode() == FbxLayerElement::eIndexToDirect) {
            const int size = col->GetIndexArray().GetCount();
            util::valloc(&this->colors, size, false);
            // インデックスから取得
            for (int k = 0; k < size; ++k) {
                const int index = col->GetIndexArray().GetAt(i);
                auto v = col->GetDirectArray().GetAt(index);
                VertexContainer::colors[k] = Color::fromRGBAf(v[0], v[1], v[2], v[3]);
            }
            return;
        }
    }
    assert(false);
}

void VertexContainer::createCoords(FbxMesh *mesh) {
    const int layerNum = mesh->GetElementUVCount();
    for (int i = 0; i < layerNum; ++i) {
        FbxLayerElementUV *uvElement = mesh->GetElementUV(i);
//! UV
        const int UVNum = uvElement->GetDirectArray().GetCount();
        const int indexNum = uvElement->GetIndexArray().GetCount();
        const int size = UVNum > indexNum ? UVNum : indexNum;

        // マッピングモード・リファレンスモード別にUV取得
        const FbxLayerElement::EMappingMode mappingMode = uvElement->GetMappingMode();
        const FbxLayerElement::EReferenceMode refMode = uvElement->GetReferenceMode();

        if (mappingMode == FbxLayerElement::eByPolygonVertex) {
            if (refMode == FbxLayerElement::eDirect) {
                // 直接取得
                for (int i = 0; i < size; ++i) {
                    vec2 v((float) uvElement->GetDirectArray().GetAt(i)[0], (float) uvElement->GetDirectArray().GetAt(i)[1]);
                    coords.push_back(v);
                }
                return;
            } else if (refMode == FbxLayerElement::eIndexToDirect) {
                // インデックスから取得
                for (int i = 0; i < size; ++i) {
                    const int index = uvElement->GetIndexArray().GetAt(i);
                    vec2 v((float) uvElement->GetDirectArray().GetAt(index)[0], (float) uvElement->GetDirectArray().GetAt(index)[1]);
                    coords.push_back(v);
                }
                return;
            } else {
                eslog("not support refmode(%d)", refMode);
                assert(false);
            }
        }
    }

    coords.shrink_to_fit();
}

void VertexContainer::createWeights(FbxMesh *mesh) {
    const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

    eslog("    - Mesh has deformers(%d)", deformerCount);
    if (!deformerCount) {
        return;
    }

    weights.clear();

    // 位置数だけ必要になる
    util::valloc(&weights, positions.size(), false);

    for (int i = 0; i < deformerCount; ++i) {
        FbxSkin *skin = (FbxSkin *) mesh->GetDeformer(i, FbxDeformer::eSkin);
        clusterCount = skin->GetClusterCount();
        eslog("    - Mesh clusters(%d) type[%d] indices(%d)",
              clusterCount,
              skin->GetSkinningType(),
              skin->GetControlPointIndicesCount());

  //! クラスタ（ボーン）を取得する
        for (int boneIndex = 0; boneIndex < clusterCount; ++boneIndex) {
            FbxCluster *cluster = skin->GetCluster(boneIndex);
            const char *culsterName = cluster->GetLink()->GetName();

            const unsigned pointsNum = cluster->GetControlPointIndicesCount();
            const int* indicesRef = cluster->GetControlPointIndices();
            const double* weightsRef = cluster->GetControlPointWeights();

            eslog("    - Cluster[%02d] id[%llu] link[%llu] name(%s) geoms(%d)",
                  boneIndex, cluster->GetUniqueID(), cluster->GetLink()->GetUniqueID(),
                  culsterName,
                  pointsNum
                  );

//            // すべてのインデックス情報をログに吐き出す
//            for (int i = 0; i < pointsNum; ++i) {
//                eslog("      - index(%d) weight(%.3f)", indicesRef[i], weightsRef[i]);
//            }


            // すべてのウェイトを頂点に加える
            for (unsigned k = 0; k < pointsNum; ++k) {
                const float weight = weightsRef[k];
                const unsigned index = indicesRef[k];
                weights[index].registerWegight(cluster, boneIndex, weight);
            }
        }
    }

    // 頂点ウェイトを正規化する
    for (auto &weight : weights) {
        weight.normalize();
    }
}

}
}