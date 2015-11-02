#include <btBulletDynamicsCommon.h>
#include <es/asset/file/SerializeHeader.h>
#include "ModelData.h"
#include "es/internal/protoground-internal.hpp"

#if defined(BUILD_Windows)
#pragma warning(disable: 4018) // unsigned <> signed比較を許容する
#endif

namespace es {
namespace file {

ByteBuffer MeshData::serialize(MeshData::Serialize *mesh) {

    const uint32_t vertexBytes = mesh->meta.vertexNum * mesh->meta.vertexComplex.vertexBytes;
    const uint32_t indicesBytes = mesh->meta.indexNum * mesh->meta.indexDataBytes;
    const uint32_t bufferSize = sizeof(mesh->meta) + vertexBytes + indicesBytes;

    ByteBuffer result = Buffer::create(bufferSize);
    assert(result.length() == bufferSize);

    uint8_t *write = result.get();
    memcpy(write, &mesh->meta, sizeof(mesh->meta));
    write += sizeof(mesh->meta);

    memcpy(write, mesh->vertices.get(), vertexBytes);
    write += vertexBytes;

    memcpy(write, mesh->indices.get(), indicesBytes);
    write += indicesBytes;

    return result;
}

bool MeshData::deserialize(MeshData::Serialize *result, const unsafe_array<uint8_t> buffer) {
    assert(result);

    uint8_t *read = buffer.ptr;
    memcpy(&result->meta, read, sizeof(result->meta));
    read += sizeof(result->meta);

    const uint32_t vertexBytes = result->meta.vertexNum * result->meta.vertexComplex.vertexBytes;
    const uint32_t indicesBytes = result->meta.indexNum * result->meta.indexDataBytes;

    // データが足りない
    if (buffer.length < (sizeof(result->meta) + vertexBytes + indicesBytes)) {
        return false;
    }

    // 頂点データを取り出す
    result->vertices = Buffer::alignNew(vertexBytes);
    memcpy(result->vertices.get(), read, vertexBytes);
    read += vertexBytes;

    // インデックスデータを取り出す
    result->indices = Buffer::alignNew(indicesBytes);
    memcpy(result->indices.get(), read, indicesBytes);
    read += indicesBytes;

    return true;
}

ByteBuffer MaterialData::serialize(MaterialData::Serialize *material) {
    assert(material);

    material->meta.materialNum = material->materials.size();
    material->meta.textureNum = material->textures.size();

    const uint32_t materialsBytes = sizeof(Material) * material->materials.size();
    const uint32_t texturesBytes = sizeof(Texture) * material->textures.size();
    const uint32_t bufferSize = sizeof(material->meta) + materialsBytes + texturesBytes;

    ByteBuffer result = Buffer::create(bufferSize);
    uint8_t *write = result.get();

    memcpy(write, &material->meta, sizeof(material->meta));
    write += sizeof(material->meta);

    memcpy(write, util::asPointer(material->materials), materialsBytes);
    write += materialsBytes;

    memcpy(write, util::asPointer(material->textures), texturesBytes);
    write += texturesBytes;

    return result;
}

bool MaterialData::deserialize(MaterialData::Serialize *result, const unsafe_array<uint8_t> buffer) {
    assert(result);

    uint8_t *read = buffer.ptr;
    memcpy(&result->meta, read, sizeof(Meta));
    read += sizeof(Meta);
    const uint32_t materialsBytes = sizeof(Material) * result->meta.materialNum;
    const uint32_t texturesBytes = sizeof(Texture) * result->meta.textureNum;

    // 容量を検証する
    if (buffer.length < (sizeof(Meta) + materialsBytes + texturesBytes)) {
        return false;
    }

    util::valloc(&result->materials, (size_t) result->meta.materialNum, false);
    memcpy(util::asPointer(result->materials), read, materialsBytes);
    read += materialsBytes;

    if (result->meta.textureNum) {
        util::valloc(&result->textures, (size_t) result->meta.textureNum, false);
        memcpy(util::asPointer(result->textures), read, texturesBytes);
        read += texturesBytes;
    }

    return true;
}

ByteBuffer BoneData::serialize(BoneData::Serialize *bone) {
    assert(bone);

    bone->meta.boneNum = bone->bones.size();
    bone->meta.ikLinkNum = bone->ikLinks.size();

    const uint32_t bonesBytes = sizeof(Bone) * bone->bones.size();
    const uint32_t ikLinkBytes = sizeof(IkLink) * bone->ikLinks.size();
    const uint32_t bufferSize = sizeof(Meta) + bonesBytes + ikLinkBytes;

    ByteBuffer result = Buffer::create(bufferSize);
    uint8_t *write = result.get();

    memcpy(write, &bone->meta, sizeof(Meta));
    write += sizeof(Meta);

    memcpy(write, util::asPointer(bone->bones), bonesBytes);
    write += bonesBytes;

    if (!bone->ikLinks.empty()) {
        memcpy(write, util::asPointer(bone->ikLinks), ikLinkBytes);
        write += bonesBytes;
    }

    return result;

}

bool BoneData::deserialize(BoneData::Serialize *result, const unsafe_array<uint8_t> buffer) {
    assert(result);

    uint8_t *read = buffer.ptr;
    memcpy(&result->meta, read, sizeof(Meta));
    read += sizeof(Meta);
    const uint32_t bonesBytes = sizeof(Bone) * result->meta.boneNum;
    const uint32_t ikLinkBytes = sizeof(IkLink) * result->meta.ikLinkNum;

    // 容量を検証する
    if (buffer.length < (sizeof(Meta) + bonesBytes + ikLinkBytes)) {
        return false;
    }

    {
        util::valloc(&result->bones, (size_t) result->meta.boneNum, false);
        memcpy(util::asPointer(result->bones), read, bonesBytes);
        read += bonesBytes;
    }

    if (result->meta.ikLinkNum) {
        util::valloc(&result->ikLinks, (size_t) result->meta.ikLinkNum, false);
        memcpy(util::asPointer(result->ikLinks), read, ikLinkBytes);
        read += ikLinkBytes;
    }

    return true;
}

ByteBuffer MorphData::serialize(MorphData::Serialize *morph) {
    assert(morph);

    morph->meta.targetNum = morph->targets.size();
    morph->meta.positionMorphNum = morph->positionMorphs.size();
    morph->meta.uvMorphNum = morph->uvMorphs.size();

    const uint32_t targetsBytes = sizeof(Target) * morph->targets.size();
    const uint32_t positionMorphsBytes = sizeof(PositionMorph) * morph->positionMorphs.size();
    const uint32_t uvMorphsBytes = sizeof(PositionMorph) * morph->uvMorphs.size();
    const uint32_t bufferSize = sizeof(Meta) + targetsBytes + positionMorphsBytes + uvMorphsBytes;

    ByteBuffer result = Buffer::create(bufferSize);
    uint8_t *write = result.get();

    memcpy(write, &morph->meta, sizeof(Meta));
    write += sizeof(Meta);

    memcpy(write, util::asPointer(morph->targets), targetsBytes);
    write += targetsBytes;

    if (!morph->positionMorphs.empty()) {
        memcpy(write, util::asPointer(morph->positionMorphs), positionMorphsBytes);
        write += positionMorphsBytes;
    }

    if (!morph->uvMorphs.empty()) {
        memcpy(write, util::asPointer(morph->uvMorphs), uvMorphsBytes);
        write += uvMorphsBytes;
    }

    return result;

}

bool MorphData::deserialize(MorphData::Serialize *result, const unsafe_array<uint8_t> buffer) {
    assert(result);

    uint8_t *read = buffer.ptr;
    memcpy(&result->meta, read, sizeof(Meta));
    read += sizeof(Meta);
    const uint32_t targetsBytes = sizeof(Target) * result->meta.targetNum;
    const uint32_t positionMorphsBytes = sizeof(PositionMorph) * result->meta.positionMorphNum;
    const uint32_t uvMorphsBytes = sizeof(PositionMorph) * result->meta.uvMorphNum;
    const uint32_t bufferSize = sizeof(Meta) + targetsBytes + positionMorphsBytes + uvMorphsBytes;

    // 容量を検証する
    if (buffer.length < bufferSize) {
        return false;
    }

    if (result->meta.targetNum) {
        util::valloc(&result->targets, (size_t) result->meta.targetNum, false);
        memcpy(util::asPointer(result->targets), read, targetsBytes);
        read += targetsBytes;
    }

    if (result->meta.positionMorphNum) {
        util::valloc(&result->positionMorphs, (size_t) result->meta.positionMorphNum, false);
        memcpy(util::asPointer(result->positionMorphs), read, positionMorphsBytes);
        read += positionMorphsBytes;
    }


    if (result->meta.uvMorphNum) {
        util::valloc(&result->uvMorphs, (size_t) result->meta.uvMorphNum, false);
        memcpy(util::asPointer(result->uvMorphs), read, uvMorphsBytes);
        read += uvMorphsBytes;
    }

    return true;
}

ByteBuffer PhysicsData::serialize(PhysicsData::Serialize *physics) {
    assert(physics);

    physics->meta.bodyNum = physics->rigidBodies.size();
    physics->meta.jointNum = physics->rigidJoints.size();

    const uint32_t bodiesBytes = sizeof(RigidBody) * physics->meta.bodyNum;
    const uint32_t jointsByes = sizeof(RigidJoint) * physics->meta.jointNum;
    const uint32_t bufferSize = sizeof(Meta) + bodiesBytes + jointsByes;

    ByteBuffer result = Buffer::create(bufferSize);
    uint8_t *write = result.get();

    memcpy(write, &physics->meta, sizeof(Meta));
    write += sizeof(Meta);

    if (!physics->rigidBodies.empty()) {
        memcpy(write, util::asPointer(physics->rigidBodies), bodiesBytes);
        write += bodiesBytes;
    }

    if (!physics->rigidJoints.empty()) {
        memcpy(write, util::asPointer(physics->rigidJoints), jointsByes);
        write += jointsByes;
    }

    return result;
}

bool PhysicsData::deserialize(PhysicsData::Serialize *result, const unsafe_array<uint8_t> buffer) {
    assert(result);

    uint8_t *read = buffer.ptr;
    memcpy(&result->meta, read, sizeof(Meta));
    read += sizeof(Meta);
    const uint32_t bodiesBytes = sizeof(RigidBody) * result->meta.bodyNum;
    const uint32_t jointsBytes = sizeof(RigidJoint) * result->meta.jointNum;
    const uint32_t bufferSize = sizeof(Meta) + bodiesBytes + jointsBytes;

    // 容量を検証する
    if (buffer.length < bufferSize) {
        return false;
    }

    if (result->meta.bodyNum) {
        util::valloc(&result->rigidBodies, (size_t) result->meta.bodyNum, false);
        memcpy(util::asPointer(result->rigidBodies), read, bodiesBytes);
        read += bodiesBytes;
    }

    if (result->meta.jointNum) {
        util::valloc(&result->rigidJoints, (size_t) result->meta.jointNum, false);
        memcpy(util::asPointer(result->rigidJoints), read, jointsBytes);
        read += jointsBytes;
    }

    return true;
}

int8_t MaterialData::getTextureNumber(const string &texName, SymbolTable *table, std::vector<MaterialData::Texture> *textures) {
    const uint64_t symbol = table->add(texName);

    int index = 0;
    for (const auto &tex : (*textures)) {
        if (tex.symbol == symbol) {
            return index;
        }
        ++index;
    }

    // 見つからないので末尾に追加
    MaterialData::Texture tex;
    tex.symbol = symbol;
    textures->push_back(tex);

    return index;
}

ByteBuffer SkinMeshModelData::serialize(SkinMeshModelData *model) {
    assert(model);

    Meta meta = {0};

    ByteBuffer meshBuffer = MeshData::serialize(&model->mesh);
    meta.meshDataBytes = meshBuffer.length();

    ByteBuffer materialBuffer = MaterialData::serialize(&model->material);
    meta.materialDataBytes = materialBuffer.length();

    ByteBuffer boneBuffer;
    if (model->bone.bones.size()) {
        boneBuffer = BoneData::serialize(&model->bone);
        meta.boneDataBytes = boneBuffer.length();
    } else {
        memset(&model->bone.meta, 0x00, sizeof(model->bone.meta));
    }

    ByteBuffer morphBuffer;
    if (model->morph.targets.size()) {
        morphBuffer = MorphData::serialize(&model->morph);
        meta.morphDataBytes = morphBuffer.length();
    } else {
        memset(&model->morph.meta, 0x00, sizeof(model->morph.meta));
    }

    ByteBuffer physicsBuffer;
    if (model->physics.rigidBodies.size()) {
        physicsBuffer = PhysicsData::serialize(&model->physics);
        meta.physicsDataBytes = physicsBuffer.length();
    } else {
        memset(&model->physics.meta, 0x00, sizeof(model->physics.meta));
    }

    // 書き込む
    const uint32_t bufferBytes = sizeof(Meta) + meta.meshDataBytes + meta.materialDataBytes + meta.boneDataBytes + meta.morphDataBytes + meta.physicsDataBytes;
    SerializeHeader header(SerializeHeader::DATA_UID_GRAPHICS_MODEL, bufferBytes);

    ByteBuffer result = Buffer::create(bufferBytes + sizeof(SerializeHeader));
    uint8_t *write = result.get();

    // ヘッダを書き込む
    memcpy(write, &header, sizeof(header));
    write += sizeof(header);

    memcpy(write, &meta, sizeof(Meta));
    write += sizeof(Meta);

    memcpy(write, meshBuffer.get(), meshBuffer.length());
    write += meshBuffer.length();

    memcpy(write, materialBuffer.get(), materialBuffer.length());
    write += materialBuffer.length();

    if (boneBuffer.length()) {
        memcpy(write, boneBuffer.get(), boneBuffer.length());
        write += boneBuffer.length();
    }

    if (morphBuffer.length()) {
        memcpy(write, morphBuffer.get(), morphBuffer.length());
        write += morphBuffer.length();
    }

    if (physicsBuffer.length()) {
        memcpy(write, physicsBuffer.get(), physicsBuffer.length());
        write += physicsBuffer.length();
    }

    return result;
}

bool SkinMeshModelData::deserialize(SkinMeshModelData *result, unsafe_array<uint8_t> buffer) {
    assert(result);

    // ヘッダチェック
    SerializeHeader header;
    if (!SerializeHeader::deserialize(&buffer, &header, SerializeHeader::DATA_UID_GRAPHICS_MODEL)) {
        eslog("HeaderDeserialize Error");
        return false;
    }

    uint8_t *read = buffer.ptr;
    Meta meta = {0};

    memcpy(&meta, read, sizeof(Meta));
    read += sizeof(Meta);
    const uint32_t bufferBytes = sizeof(Meta) + meta.meshDataBytes + meta.materialDataBytes + meta.boneDataBytes + meta.morphDataBytes + meta.physicsDataBytes;

    if (buffer.length < bufferBytes) {
        return false;
    }

    {
        unsafe_array<uint8_t> buffer(read, meta.meshDataBytes);
        if (!MeshData::deserialize(&result->mesh, buffer)) {
            return false;
        }
        read += meta.meshDataBytes;
    }

    {
        unsafe_array<uint8_t> buffer(read, meta.materialDataBytes);
        if (!MaterialData::deserialize(&result->material, buffer)) {
            return false;
        }
        read += meta.materialDataBytes;
    }

    if (meta.boneDataBytes) {
        unsafe_array<uint8_t> buffer(read, meta.boneDataBytes);
        if (!BoneData::deserialize(&result->bone, buffer)) {
            return false;
        }
        read += meta.boneDataBytes;
    } else {
        memset(&result->bone.meta, 0x00, sizeof(result->bone.meta));
    }

    if (meta.morphDataBytes) {
        unsafe_array<uint8_t> buffer(read, meta.morphDataBytes);
        if (!MorphData::deserialize(&result->morph, buffer)) {
            return false;
        }
        read += meta.morphDataBytes;
    } else {
        memset(&result->morph.meta, 0x00, sizeof(result->morph.meta));
    }

    if (meta.physicsDataBytes) {
        unsafe_array<uint8_t> buffer(read, meta.physicsDataBytes);
        if (!PhysicsData::deserialize(&result->physics, buffer)) {
            return false;
        }
        read += meta.physicsDataBytes;
    } else {
        memset(&result->physics.meta, 0x00, sizeof(result->physics.meta));
    }

    return true;
}

void MeshData::calcMeta(MeshData::Serialize *serialize) {
    assert(serialize && serialize->vertices);

    uint8_t *read = serialize->vertices.get();
    uint32_t offset = serialize->meta.vertexComplex.vertexBytes;
    uint32_t numVertices = serialize->meta.vertexNum;

    vec3 minPos(9999999, 9999999, 9999999);
    vec3 maxPos(-9999999, -9999999, -9999999);

    for (int i = 0; i < numVertices; ++i) {
        vec3 *pos = (vec3 *) read;
        minPos.x = std::min<float>(pos->x, minPos.x);
        minPos.y = std::min<float>(pos->y, minPos.y);
        minPos.z = std::min<float>(pos->z, minPos.z);

        maxPos.x = std::max<float>(pos->x, maxPos.x);
        maxPos.y = std::max<float>(pos->y, maxPos.y);
        maxPos.z = std::max<float>(pos->z, maxPos.z);

        read += offset;
    }

    serialize->meta.aabb.maxPos = maxPos;
    serialize->meta.aabb.minPos = minPos;
}

}
}