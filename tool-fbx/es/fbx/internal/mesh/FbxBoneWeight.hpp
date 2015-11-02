#pragma once

#include "es/protoground-fbx.hpp"

namespace es {
namespace fbx {

struct FbxBoneWeight {
    enum {
        WEIGHT_NUM = 4,
        UNUSED_BONE = 0xFFFF
    };

    FbxBoneWeight() {
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            indices[i] = UNUSED_BONE;
            weights[i] = 0;
            clusters[i] = nullptr;
        }
    }

    /**
     * ウェイト値を書き込む
     */
    void registerWegight(FbxCluster *cluster, uint16_t boneIndex, float weight) {
        float lowerWeight = 99999.0f; // 最も小さいウェイト値
        int lowerWeightIndex = 0; // 最も小さいウェイト値のインデックス
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            // 登録されていないインデックスがあればそこへ書き込む
            if (indices[i] == UNUSED_BONE) {
                indices[i] = boneIndex;
                weights[i] = weight;
                clusters[i] = cluster;
                return;
            }

            if (weights[i] < lowerWeight) {
                lowerWeight = weights[i];
                lowerWeightIndex = i;
            }
        }

        //! もっとも低いウェイトを切り捨てる。
        if (weight > lowerWeight) {
            indices[lowerWeightIndex] = boneIndex;
            weights[lowerWeightIndex] = weight;
            clusters[lowerWeightIndex] = cluster;
        }
    }

    /**
     * 頂点ウェイト値を最適化する
     */
    void normalize() {
        float sum = 0.0f;
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            sum += weights[i];
        }

        // 合計値が0の場合は何もしない
        if (sum == 0) {
            return;
        }

        for (int i = 0; i < WEIGHT_NUM; ++i) {
            weights[i] /= sum;
            if (indices[i] == UNUSED_BONE) {
                indices[i] = 0;
            }
        }

        for (int i = 0; i < (WEIGHT_NUM - 1); ++i) {
            for (int k = (i + 1); k < WEIGHT_NUM; ++k) {
                if (weights[i] < weights[k]) {
                    // 値が大きい方が若いインデックスに来て欲しい
                    std::swap(weights[i], weights[k]);
                    std::swap(indices[i], indices[k]);
                    std::swap(clusters[i], clusters[k]);
                }
            }
        }
    }

    /**
     * 参照するボーンのインデックス.
     *
     * 十分なボーン数を利用できるようになったため、インデックスはushortで許可する。
     */
    uint16_t indices[WEIGHT_NUM];

    /**
     * ボーンの重み係数.
     * 合計値は必ず1.0fになる。
     */
    float weights[WEIGHT_NUM];

    /**
     * クラスタリスト
     */
    FbxCluster *clusters[WEIGHT_NUM];
};


}
}