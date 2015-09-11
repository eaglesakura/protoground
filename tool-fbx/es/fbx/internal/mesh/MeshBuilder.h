#pragma once

#include "es/protoground-fbx.hpp"
#include "FbxBoneWeight.hpp"
#include <es/system/Hash128.h>
#include <es/graphics/Color.hpp>
#include "PolygonContainer.h"
#include "VertexContainer.h"
#include <map>
#include <vector>
#include <es/memory/Buffer.hpp>

#include "es/graphics/model/file/ModelData.h"
#include "FbxNodeTree.h"

namespace es {
namespace fbx {

class FbxNodeTree;

/**
 * 変換用の頂点情報を構築する
 */
class MeshBuilder : public Object {
public:
    struct Vertex;
    struct Material;

    MeshBuilder();

    /**
     * データを追加する
     *
     * MEMO: 追加時に3dsMax座標系(Z-UP)をOpenGL右手座標系(Y-UP)に変換する
     */
    virtual void add(const std::shared_ptr<FbxNodeTree> rootNode, const FbxNodeTree *self, const VertexContainer *vertices, const PolygonContainer *polygons);

    /**
     * メッシュ情報を計算する
     */
    void computeMesh(es::file::SkinMeshModelData *result, file::SymbolTable *table) const;

    /**
     * ボーン情報を計算する
     */
    void computeBones(const std::shared_ptr<FbxNodeTree> rootNode, es::file::SkinMeshModelData *result, file::SymbolTable *table) const;

    /**
     * 頂点リストを取得する
     */
    void getVertices(std::vector<Vertex> *result) const;

    /**
     * インデックスバッファを変換して取得する
     *
     * 頂点数に合わせて、u8/u16/u32の何れかの型となる。
     */
    void getIndexBuffer(ByteBuffer *resultBuffer, uint32_t *resultIndexByte, uint32_t *resultIndicesNum) const;

    uint32_t getVertexCount() const;

    uint16_t getMaterialCount() const;

    struct Material {
        /**
         * レンダリングする頂点数
         */
        uint32_t indicesNum;

        /**
         * マテリアル情報
         */
        PolygonContainer::Material material;
    };

    struct Vertex {
        /**
         * 位置
         */
        vec3 position;

        /**
         * 法線
         */
        vec3 normal;

        /**
         * UV
         */
        vec2 coord;

        /**
         * ボーンウェイト情報
         */
        uint8_t weights[FbxBoneWeight::WEIGHT_NUM];

        /**
         * ボーンインデックス情報
         */
        uint16_t indices[FbxBoneWeight::WEIGHT_NUM];

        /**
         * 頂点カラー
         */
        es::Color color;

        /**
         * ハッシュを生成する
         */
        Hash128 hash() const;

        string hashString() const;

        /**
         * 検証用
         */
        bool operator==(const Vertex &v) const;
    };

    virtual ~MeshBuilder() = default;

private:
    struct MeshGroup {
        /**
         * 頂点インデックスリスト
         */
        std::vector<uint32_t> indices;
    };

    /**
     * 頂点番号を探すか、頂点を生成する
     */
    uint32_t findVertexIndexOrCreate(const Vertex &v);

    /**
     * 登録されているクラスタを探す
     */
    uint16_t findClusterIndexOrCreate(const std::shared_ptr<FbxNodeTree> rootNode, const FbxCluster *cluster);

    /**
     * マテリアルを探すか、マテリアルを生成する
     */
    uint16_t findMaterialIndexOrCreate(const PolygonContainer::Material &mtl);

    /**
     * 内部で管理される頂点情報
     */
    struct VertexData {
        /**
         * 頂点データ本体
         */
        Vertex vertex;

        /**
         * 頂点のハッシュ情報
         */
        Hash128 hash;

        /**
         * 頂点番号
         */
        uint32_t number;
    };

    struct MaterialData {
        /**
         * データ本体
         */
        PolygonContainer::Material material;

        /**
         * マテリアル由来のハッシュ情報
         */
        Hash128 hash;

        /**
         * マテリアルナンバー
         */
        uint16_t number;
    };

    struct ClusterData {
        /**
         * ボーンとして動作するクラスタ
         */
        FbxNode *linkedNode;

        /**
         * ボーンのハッシュ値
         */
        Hash128 hash;

        /**
         * ボーン番号
         */
        uint16_t number;
    };

    /**
     * 頂点データ本体
     * 同一頂点を検索できるよう、ハッシュで登録する
     */
    std::map<Hash128, VertexData> vertices;

    /**
     * 生成されたマテリアル情報
     */
    std::map<Hash128, MaterialData> materials;

    /**
     * マテリアルごとのメッシュリスト
     * メッシュ0番から整列される
     */
    std::vector<sp<MeshGroup> > meshList;
};

}
}


