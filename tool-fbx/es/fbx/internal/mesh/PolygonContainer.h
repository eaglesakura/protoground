#pragma once

#include "es/protoground-fbx.hpp"
#include <vector>
#include <es/graphics/Color.hpp>
#include <es/system/Hash128.h>

namespace es {
namespace fbx {

class PolygonContainer {
public:
    /**
     * マテリアル情報
     */
    struct Material {
        /**
         * マテリアル名
         */
        string name;

        /**
         * 表面色
         */
        Color diffuse;

        /**
         * テクスチャ名
         */
        string textureName;

        /**
         * 一意のハッシュコードを生成する
         */
        Hash128 hash() const;
    };

    /**
     * ポリゴン情報
     */
    struct Polygon {
        /**
         * 位置・法線
         */
        uint32_t position[3];

        /**
         * UV/Normal座標
         */
        uint32_t attributes[3];

        /**
         * マテリアル番号
         */
        uint32_t material;
    };

    /**
     * マテリアル一覧
     */
    std::vector<Material> materials;

    /**
     * 管理しているポリゴン情報一覧
     */
    std::vector<Polygon> polygons;

    void build(FbxMesh *mesh);

private:
    void createMaterials(FbxMesh *mesh);

    void createPolygonList(FbxMesh *mesh);
};


}
}

