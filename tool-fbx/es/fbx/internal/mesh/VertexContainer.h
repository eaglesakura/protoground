#pragma once

#include "es/protoground-fbx.hpp"
#include "FbxBoneWeight.hpp"
#include    <vector>
#include <es/graphics/Color.hpp>

namespace es {
namespace fbx {

/**
 * FBXの頂点を一元管理する
 * FBXが保持していたままの配列のため、各配列数が一致するとは限らない。
 *
 * MEMO: 一致するはず
 * positions.size == weights.size
 * normals.size == coords.size
 */
class VertexContainer {
public:
    /**
     * 位置情報
     */
    std::vector<vec3> positions;

    /**
     * 法線
     */
    std::vector<vec3> normals;

    /**
     * 色情報
     */
    std::vector<es::Color> colors;

    /**
     * UV情報
     */
    std::vector<vec2> coords;

    /**
     * 頂点の重み情報
     */
    std::vector<FbxBoneWeight> weights;

    /**
     * ボーン数
     */
    uint16_t clusterCount  = 0;

    /**
     * 生成された頂点リストが正しいことを検証する
     */
    bool valid() {
        return (weights.empty() ? true :  positions.size() == weights.size())
               && normals.size() == coords.size()
               && normals.size() == colors.size();
    }

    void build(FbxMesh *mesh);

private:
    /**
     * 位置情報を生成する
     */
    void createPositions(FbxMesh *mesh);

    /**
     * 法線を生成する
     */
    void createNormals(FbxMesh *mesh);

    /**
     * 色を生成する
     */
    void createColors(FbxMesh *mesh);

    /**
     * UVを生成する
     */
    void createCoords(FbxMesh *mesh);

    /**
     * ボーンウェイトを生成する
     */
    void createWeights(FbxMesh *mesh);
};
}
}