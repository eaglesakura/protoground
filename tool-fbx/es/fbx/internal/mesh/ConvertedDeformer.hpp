#pragma once

#include "es/protoground-fbx.hpp"
#include <vector>
#include <es/memory/SafeArray.hpp>
#include <es/util/StringUtil.h>

namespace es {
namespace fbx {

/**
 * FBXの１メッシュ毎に複数のDeformerが作成される。
 *
 * Deformerは特定の頂点に対して影響を及ぼす、ボーンとしての役割がある。
 *
 * indices[num]とweights[num]はセットになっていて、indices[num]の頂点に対してweights[num]を与える。
 */
class ConvertedDeformer {
public:
    /**
     * 頂点番号テーブル
     */
    std::vector<uint32_t> indices;

    /**
     * 重みテーブル
     */
    std::vector<float> weights;

    ConvertedDeformer(FbxCluster *cluster) {
        createBoneIndicesArray(cluster);
        createBoneWeightArray(cluster);

    }

    ~ConvertedDeformer() = default;

    /**
     * クラスタ名を取得する
     */
    static string getName(FbxCluster *cluster) {
        if (strlen(cluster->GetName())) {
            return cluster->GetName() + (strlen("Cluster_"));
        } else {
            return util::format("Cluster[%llu]", cluster->GetUniqueID());
        }
    }

private:
    void createBoneIndicesArray(FbxCluster *cluster) {
        int *origin = cluster->GetControlPointIndices();
        int num = cluster->GetControlPointIndicesCount();

        if (num) {
            util::valloc(&indices, num, false);
            memcpy(util::asPointer(indices), origin, sizeof(int) * num);
        }
    }

    void createBoneWeightArray(FbxCluster *cluster) {
        double *origin = cluster->GetControlPointWeights();
        int num = cluster->GetControlPointIndicesCount();

        if (num) {
            util::valloc(&weights, num, false);
            for (int i = 0; i < num; ++i) {
                weights[i] = (float) origin[i];
            }
        }

    }

};
}
}