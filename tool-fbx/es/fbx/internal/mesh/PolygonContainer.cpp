#include <es/util/StringUtil.h>
#include <es/internal/log/Log.h>
#include <es/graphics/Color.hpp>
#include "PolygonContainer.h"

namespace es {
namespace fbx {


void PolygonContainer::build(FbxMesh *mesh) {
    createMaterials(mesh);
    createPolygonList(mesh);
}

void PolygonContainer::createMaterials(FbxMesh *mesh) {
    const int materialNum = mesh->GetNode()->GetMaterialCount();

    Material mtl;
    if (materialNum == 0) {
        mtl.name = "NO_MATERIAL";
        materials.push_back(mtl);
        return;
    }

// マテリアルを全て集積する
    util::valloc(&materials, materialNum, false);
    for (int i = 0; i < materialNum; ++i) {
        FbxSurfaceMaterial *material = mesh->GetNode()->GetMaterial(i);
        mtl.name = material->GetName();

        if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
            FbxSurfaceLambert *mat = (FbxSurfaceLambert *) material;
            mtl.diffuse = Color::fromRGBAf(mat->Diffuse.Get()[0], mat->Diffuse.Get()[1], mat->Diffuse.Get()[2], (float) mat->DiffuseFactor);
        } else if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
            FbxSurfacePhong *mat = (FbxSurfacePhong *) material;
            mtl.diffuse = Color::fromRGBAf(mat->Diffuse.Get()[0], mat->Diffuse.Get()[1], mat->Diffuse.Get()[2], (float) mat->DiffuseFactor);
        }
        FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
        const int texNum = prop.GetSrcObjectCount<FbxFileTexture>();
        if (texNum) {
            FbxFileTexture *texture = prop.GetSrcObject<FbxFileTexture>(0);
            if (texture && texture->GetFileName()) {

                string fileName = util::getFileName(texture->GetFileName());
                int dot_index = fileName.find('.');
                if (dot_index > 0) {
                    fileName = fileName.substr(0, dot_index);
                }
                mtl.textureName = fileName;
            }
        }

        eslog("    - Mat(%s) tex name(%s)", mtl.name.c_str(), mtl.textureName.c_str());
        materials[i] = mtl;
    }
}

namespace {
//! メッシュ内の三角形数を取得する
static int getTriangleCount(FbxMesh *mesh) {
    int ret = 0;
    int polyNum = mesh->GetPolygonCount();

    for (int i = 0; i < polyNum; ++i) {
        int tris = mesh->GetPolygonSize(i) - 2;
        // 三角ポリゴンのみを許可する
        // 既に三角分割を行っているため、ここは必ず通過するはずである。
        assert(tris == 1);
        ret += tris;
    }
    return ret;
}

static std::vector<uint16_t> createMaterialIndexArray(FbxMesh *mesh, int layerNum) {
    std::vector<uint16_t> result;
    FbxLayerElementMaterial *mat = mesh->GetLayer(layerNum)->GetMaterials();
    if (!mat) {
        return result;
    }

    const auto refMode = mat->GetReferenceMode();
    if (refMode != FbxLayerElementMaterial::eIndexToDirect) {
        return result;
    }

    int indexArrayCount = mat->GetIndexArray().GetCount();
    util::valloc(&result, indexArrayCount, false);
    // 参照取得
    for (int i = 0; i < indexArrayCount; ++i) {
        int index = mat->GetIndexArray().GetAt(i);
        result[i] = (uint16_t) (std::max(index, 0));
    }

    return result;
}
}

void PolygonContainer::createPolygonList(FbxMesh *mesh) {
// 必要なポリゴン数
    int triangleNum = getTriangleCount(mesh);
// quadを含んだFBXポリゴン数
    int polyNum = mesh->GetPolygonCount();
//! マテリアル配列を作成する。
    auto materialNumbers = createMaterialIndexArray(mesh, 0);

    int current = 0;
    int uvIndex = 0;

    util::valloc(&polygons, polyNum, false);

//! 三角形の位置情報はインデックスで、UV情報はポリゴンごとに順に格納されている。
    for (int i = 0; i < polyNum; ++i) {
        int size = mesh->GetPolygonSize(i);
        std::vector<uint32_t> index(size);

        for (int k = 0; k < size; ++k) {
            index[k] = mesh->GetPolygonVertex(i, k);
        }

        // 四角形ポリは三角形ポリに分解して登録する
        for (int k = 0; k < (size - 2); ++k) {
            assert(current < (int) polygons.size());

            //! 頂点インデックス
            polygons[current].position[0] = index[0];
            polygons[current].position[1] = index[k + 2];
            polygons[current].position[2] = index[k + 1];
            //! uv
            polygons[current].attributes[0] = (uvIndex + 0);
            polygons[current].attributes[1] = (uvIndex + k + 2);
            polygons[current].attributes[2] = (uvIndex + k + 1);

//            jclogf("uvindex(%d, %d, %d)", (uvIndex + 0), (uvIndex + k + 2), (uvIndex + k + 1));

//! マテリアル番号
            if ((int) materialNumbers.size() <= i) {
                polygons[current].material = 0;
//                throw create_exception_t(FbxException, FbxException_MaterialNotFound);
            } else {
                polygons[current].material = materialNumbers[i];
            }
            assert(polygons[current].material < materials.size());
            current++;
        }
        uvIndex += size;
    }

// 不具合
    assert(current == triangleNum);
    eslog("    - Triangles(%d)", polygons.size());
}

Hash128 PolygonContainer::Material::hash() const {
    auto src = util::format(1024 + name.length() + textureName.length(),
                            "name(%s)/dfs(%d,%d,%d,%d)/tname(%s)",
                            name.c_str(),
                            (int) diffuse.tag.r, (int) diffuse.tag.g, (int) diffuse.tag.b, (int) diffuse.tag.a,
                            textureName.c_str()
    );
    return Hash128::from(src);
}
}
}