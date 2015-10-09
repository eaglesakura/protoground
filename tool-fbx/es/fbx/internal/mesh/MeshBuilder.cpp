#include <es/util/StringUtil.h>
#include <es/internal/log/Log.h>
#include <es/graphics/model/file/ModelData.h>
#include "MeshBuilder.h"
#include "FbxNodeTree.h"

namespace es {
namespace fbx {

MeshBuilder::MeshBuilder() {

}

void MeshBuilder::add(const std::shared_ptr<FbxNodeTree> rootNode, const FbxNodeTree *self, const VertexContainer *vertices, const PolygonContainer *polygons) {
    assert((bool) rootNode);
    // ポリゴンリスト内部のローカルなリストから、全体の番号へ変換する紐付け
    std::vector<uint16_t> materialIndexTable(polygons->materials.size());
    std::vector<uint16_t> clusterIndexTable(vertices->clusterCount);
    memset(util::asPointer(clusterIndexTable), 0x00, sizeof(uint16_t) * vertices->clusterCount);

    eslog("    BuildMesh(%s)", self->getName().c_str());
    // 全てのマテリアルを登録する
    {
        int index = 0;
        for (const auto &mtl : polygons->materials) {
            materialIndexTable[index] = findMaterialIndexOrCreate(mtl);

            eslog("    - Mtl(%s) LocalIndex[%02d] -> GlobalIndex[%02d]", mtl.name.c_str(), index, materialIndexTable[index]);
            ++index;
        }
    }
    
    // 全てのボーンを登録する
    {
        for (const auto &weight : vertices->weights) {
            for (int i = 0; i < FbxBoneWeight::WEIGHT_NUM; ++i) {
                const uint clusterLocalIndex = weight.indices[i];
                const FbxCluster *localCluster = weight.clusters[i];

                if (localCluster) {
                    clusterIndexTable[clusterLocalIndex] = findClusterIndexOrCreate(rootNode, localCluster);
//                    eslog("    - Cluster v[%d] LocalIndex[%03d] -> GlobalIndex[%03d]", i, clusterLocalIndex, clusterIndexTable[clusterLocalIndex]);
                }
            }
        }
    }

    // ポリゴン単位で頂点を構築していく
    {
        for (const auto &poly : polygons->polygons) {
            Vertex vertex[3];
            for (int i = 0; i < 3; ++i) {
                Vertex &dst = vertex[i];
                dst.position = vertices->positions[poly.position[i]];
                dst.normal = vertices->normals[poly.attributes[i]];
                dst.coord = vertices->coords[poly.attributes[i]];
                dst.color = vertices->colors[poly.attributes[i]];

                const auto &srcWeight = vertices->weights[poly.position[i]];
                int weightSumCheck = 255;
                for (int k = 0; k < FbxBoneWeight::WEIGHT_NUM; ++k) {
                    const int localIndex = srcWeight.indices[k];
                    dst.indices[k] = clusterIndexTable[localIndex];
                    dst.weights[k] = (uint8_t) (srcWeight.weights[k] * 255.0f);

                    weightSumCheck -= dst.weights[k];
                }
                assert(weightSumCheck >= 0);

                // weightが0.5+0.5の場合、127+127=254になって、ウェイト合計が255にならなくなる。
                // 合計値が255になるように誤差調整
                if (dst.weights[0] > 0) {
                    dst.weights[0] += weightSumCheck;
                }
            }

            // 書き込み先のマテリアルからメッシュグループを取得
            const uint16_t globalMaterialIndex = materialIndexTable[poly.material];
            auto mesh = meshList[globalMaterialIndex];

            // 三点のインデックスを取得する
            const uint32_t v0 = findVertexIndexOrCreate(vertex[0]);
            const uint32_t v1 = findVertexIndexOrCreate(vertex[1]);
            const uint32_t v2 = findVertexIndexOrCreate(vertex[2]);

//            eslog("    - Poly mtl[%d] v0(%d) v1(%d) v2(%d)", poly.material, v0, v1, v2);

            mesh->indices.push_back(v0);
            mesh->indices.push_back(v1);
            mesh->indices.push_back(v2);
        }
    }
}

uint32_t MeshBuilder::findVertexIndexOrCreate(const MeshBuilder::Vertex &v) {
    auto hash = v.hash();
    hash = Hash128::from((int) vertices.size());

    auto itr = vertices.find(hash);
    if (itr == vertices.end()) {
        // 新たに頂点を生成する
        VertexData d;
        d.vertex = v;
        d.hash = hash;
        d.number = vertices.size();

        vertices.insert(std::make_pair(hash, d));
        return d.number;
    } else {
//        auto v2 = itr->second.vertex;
//        if (!(v == v2)) {
//            eslog("v1 pos(%f, %f, %f) norm(%f, %f, %f) uv(%f, %f)",
//                  v.position.x, v.position.y, v.position.z,
//                  v.normal.x, v.normal.y, v.normal.z,
//                  v.coord.x, v.coord.y
//            );
//            eslog("v1 pos(%f, %f, %f) norm(%f, %f, %f) uv(%f, %f)",
//                  v2.position.x, v2.position.y, v2.position.z,
//                  v2.normal.x, v2.normal.y, v2.normal.z,
//                  v2.coord.x, v2.coord.y
//            );
//
//            assert(v == v2);
//        }
        // 既に頂点が見つかっているので番号を返す
        return itr->second.number;
    }
}

uint16_t MeshBuilder::findMaterialIndexOrCreate(const PolygonContainer::Material &mtl) {
    auto hash = mtl.hash();

    auto itr = materials.find(hash);
    if (itr == materials.end()) {
        // 新たに頂点を生成する
        MaterialData d;
        d.material = mtl;
        d.hash = hash;
        d.number = materials.size();

        materials.insert(std::make_pair(hash, d));

        // メッシュの書き込み先も作る
        meshList.push_back(sp<MeshBuilder::MeshGroup>(new MeshBuilder::MeshGroup()));
        return d.number;
    } else {
        // 既に頂点が見つかっているので番号を返す
        return itr->second.number;
    }
}

static int64_t toFixedFloat(const float f) {
    return (int64_t) (((double) f) * (float) (0xFFFFFF));
}

string MeshBuilder::Vertex::hashString() const {
    return util::format(2048,
                        "pos(%lld,%lld,%lld)/norm(%lld,%lld,%lld)/uv(%lld,%lld)/col(%d,%d,%d,%d)/idx(%d,%d,%d,%d)/wt(%d,%d,%d,%d)",
                        toFixedFloat(position.x), toFixedFloat(position.y), toFixedFloat(position.z),
                        toFixedFloat(normal.x), toFixedFloat(normal.z), toFixedFloat(normal.z),
                        toFixedFloat(coord.x), toFixedFloat(coord.y),
                        (int) color.tag.r, (int) color.tag.g, (int) color.tag.b, (int) color.tag.a,
                        (int) indices[0], (int) indices[1], (int) indices[2], (int) indices[3],
                        (int) weights[0], (int) weights[1], (int) weights[2], (int) weights[3]
    );
}

Hash128 MeshBuilder::Vertex::hash() const {
    return Hash128::from(hashString());
}

bool MeshBuilder::Vertex::operator==(const MeshBuilder::Vertex &v) const {
//    return memcmp(this, &v, sizeof(MeshBuilder::Vertex));
    return hashString() == v.hashString();
}

uint16_t MeshBuilder::findClusterIndexOrCreate(const std::shared_ptr<FbxNodeTree> rootNode, const FbxCluster *cluster) {
    auto *link = cluster->GetLink();
    auto node = rootNode->findNodeFromFbxId(link->GetUniqueID());
    assert(node);
    assert(node->getFbxUniqueId() == link->GetUniqueID());
    assert(node->getFbxNodeRef() == link);
    return node->getNodeId();
}

uint32_t MeshBuilder::getVertexCount() const {
    return vertices.size();
}

uint16_t MeshBuilder::getMaterialCount() const {
    return materials.size();
}

void MeshBuilder::getVertices(std::vector<MeshBuilder::Vertex> *result) const {
    assert(result);

    util::valloc(result, getVertexCount(), false);
    Vertex *dstTable = util::asPointer(*result);

    auto itr = vertices.begin();
    const auto end = vertices.end();

    // 全てのデータを書き込む
    while (itr != end) {
        const auto &data = itr->second;
        auto &dst = dstTable[data.number];
        dst = data.vertex;

        // MEMO 3ds座標変換を行う
        dst.position = util::to3dsAxis(dst.position);
        dst.normal = util::to3dsAxis(dst.normal);
        dst.coord = vec2(dst.coord.x, 1.0f - dst.coord.y);

        ++itr;
    }
}

void MeshBuilder::getIndexBuffer(ByteBuffer *resultBuffer, uint32_t *resultIndexByte, uint32_t *resultIndicesNum) const {
    assert(resultBuffer);
    assert(resultIndexByte);
    assert(resultIndicesNum);

    uint32_t indexBytes = 0;
    uint32_t indicesNum = 0;
    ByteBuffer convertBuffer;
    // インデックス数を数える
    uint32_t mtlIndex = 0;
    for (const auto &mesh : meshList) {
        eslog("    - Mtl[%d] index(%d) -> index(%d)", mtlIndex, indicesNum, mesh->indices.size());
        indicesNum += mesh->indices.size();
        ++mtlIndex;
    }
    assert((indicesNum % 3) == 0);

    // 必要最低限の容量を与えてコピーする
    if (indicesNum <= 0xFF) {
        indexBytes = 1;
        convertBuffer = Buffer::create(indicesNum * indexBytes);
        uint8_t *head = convertBuffer.get();
        for (const auto &mesh : meshList) {
            auto src = util::unsafe(mesh->indices);
            while (src) {
                *head = (uint8_t) (*src.ptr);
                ++src;
                ++head;
            }
        }
    } else if (indicesNum <= 0xFFFF) {
        indexBytes = 2;
        convertBuffer = Buffer::create(indicesNum * indexBytes);
        uint16_t *head = (uint16_t *) convertBuffer.get();
        uint32_t mtlIndex = 0;
        for (const auto &mesh : meshList) {
            auto src = util::unsafe(mesh->indices);
            while (src) {
                *head = (uint16_t) (*src.ptr);

//                eslog("Indices mtl[%d] [%d]", mtlIndex, (int) *head);

                ++src;
                ++head;
            }
            ++mtlIndex;
        }
    } else {
        indexBytes = 4;
        convertBuffer = Buffer::create(indicesNum * indexBytes);
        uint32_t *head = (uint32_t *) convertBuffer.get();
        for (const auto &mesh : meshList) {
            memcpy(head, util::asPointer(mesh->indices), sizeof(uint32_t) * mesh->indices.size());
            head += mesh->indices.size();
        }
    }
    eslog("    - IndexBuffer bytes(%d) num(%d) = poly(%d)", indexBytes, indicesNum, indicesNum / 3);

    *resultBuffer = convertBuffer;
    *resultIndexByte = indexBytes;
    *resultIndicesNum = indicesNum;
}

void MeshBuilder::computeMesh(es::file::SkinMeshModelData *result, file::SymbolTable *table) const {
    assert(result);

    // 組み合わせを生成して頂点本体を書き込む
    {
        auto &mesh = result->mesh;

        VertexAttribute attr[] = {
                VertexAttribute::POSITION_float3,
                VertexAttribute::NORMAL_float3,
                VertexAttribute::UV_float2,
                VertexAttribute::BONE_WEIGHT_i4,
                VertexAttribute::BONE_INDEX_i4,
                VertexAttribute::COLOR_i4,
                VertexAttribute::END
        };
        mesh.meta.vertexComplex = VertexAttribute::makeComplex(attr);
        assert(mesh.meta.vertexComplex.vertexBytes == sizeof(MeshBuilder::Vertex));

        // 頂点情報を生成
        std::vector<MeshBuilder::Vertex> vertices;
        getVertices(&vertices);
        mesh.meta.vertexNum = vertices.size();
        mesh.vertices = Buffer::alignNew(sizeof(MeshBuilder::Vertex) * vertices.size());
        memcpy(mesh.vertices.get(), util::asPointer(vertices), sizeof(MeshBuilder::Vertex) * vertices.size());

        // インデックス情報を生成
        struct {
            ByteBuffer buffer;
            uint32_t indicesNum = 0;
            uint32_t indexDataBytes = 0;
        } indices;
        getIndexBuffer(&indices.buffer, &indices.indexDataBytes, &indices.indicesNum);
        mesh.indices = indices.buffer.getSharedBuffer();
        mesh.meta.indexDataBytes = (uint8_t) indices.indexDataBytes;
        mesh.meta.indexNum = indices.indicesNum;
    }

    // メッシュを生成する
    {
        auto &material = result->material;
        material.materials.clear();

        uint32_t sumIndices = 0;

        util::valloc(&material.materials, materials.size(), false);

        for (const auto &itr : materials) {
            const auto &src = itr.second;
            const auto &mesh = meshList[src.number];

            file::MaterialData::Material &dst = material.materials[src.number];
            dst.symbol = table->add(src.material.name);
            dst.diffuse.r = src.material.diffuse.tag.r;
            dst.diffuse.g = src.material.diffuse.tag.g;
            dst.diffuse.b = src.material.diffuse.tag.b;
            dst.diffuse.a = src.material.diffuse.tag.a;
            dst.indicesNum = mesh->indices.size();
            dst.textureIndex = file::MaterialData::getTextureNumber(src.material.textureName, table, &material.textures);

            sumIndices += dst.indicesNum;
        }

        material.meta.textureNum = material.textures.size();
        material.meta.materialNum = material.materials.size();

        material.textures.shrink_to_fit();

        // インデックス数が合致しなければならない
        assert(sumIndices == result->mesh.meta.indexNum);
    }

    // メタ情報を更新する
    file::MeshData::calcMeta(&result->mesh);
    eslog("AABB min(%.3f, %.3f, %.3f) max(%.3f, %.3f, %.3f)",
          result->mesh.meta.aabb.minPos.x, result->mesh.meta.aabb.minPos.y, result->mesh.meta.aabb.minPos.z,
          result->mesh.meta.aabb.maxPos.x, result->mesh.meta.aabb.maxPos.y, result->mesh.meta.aabb.maxPos.z
    );
}

void MeshBuilder::computeBones(const std::shared_ptr<FbxNodeTree> rootNode, es::file::SkinMeshModelData *result, file::SymbolTable *table) const {
    assert(result);
    assert(rootNode);

    uint32_t nodeNum = rootNode->getNodeNum();
    assert(nodeNum > 0);

    // ノード数だけボーンを作る
    auto &bone = result->bone;
    bone.meta.boneNum = nodeNum;
    bone.meta.ikLinkNum = 0; // IKは非対応

    // ボーンを生成する
    util::valloc(&bone.bones, nodeNum, false);
    uint16_t nodeId = 0;
    for (auto &dst : bone.bones) {
        const auto src = rootNode->findNodeFromId(nodeId);
        assert(src);

        dst.pos = src->getDefaultTranslate();
        dst.rotate = src->getDefaultRotate();
        dst.symbol = table->add(src->getName());
        {
            const auto parent = src->lockParentNode();
            dst.parentBoneIndex = parent ? parent->getNodeId() : -1;
        }
        dst.ikTargetBoneIndex = -1;
        dst.ikLimitedRadian = 0;
        dst.ikLinkOffset = 0;
        dst.ikLinkNum = 0;
        dst.ikLoopCount = 0;

        ++nodeId;
    }
}

}
}