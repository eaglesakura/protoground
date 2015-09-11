#pragma once

#include "estest/protoground-test.hpp"
#include "es/graphics/model/file/SymbolTable.h"
#include "es/graphics/model/file/ModelData.h"
#include "es/graphics/model/file/AnimationData.h"

namespace es {
namespace test {

inline bool equals(const file::MeshData::Serialize *serialize, const file::MeshData::Serialize deserialize) {
    // メモリ内容が合致することを判定する
    assert(memcmp(&serialize->meta, &deserialize.meta, sizeof(serialize->meta)) == 0);

    const uint32_t verticesBytes = serialize->meta.vertexComplex.vertexBytes * serialize->meta.vertexNum;
    const uint32_t indicesBytes = serialize->meta.indexDataBytes * serialize->meta.indexNum;

    assert(memcmp(serialize->vertices.get(), deserialize.vertices.get(), verticesBytes) == 0);
    assert(memcmp(serialize->indices.get(), deserialize.indices.get(), indicesBytes) == 0);

    return true;
}

inline bool testMeshSerialize(file::MeshData::Serialize *serialize) {
    file::MeshData::Serialize deserialize;

    ByteBuffer buffer = file::MeshData::serialize(serialize);
    assert(!buffer.empty());
    assert(file::MeshData::deserialize(&deserialize, buffer.unsafe()));
    assert(equals(serialize, deserialize));
    return true;
}

inline bool equals(const file::MaterialData::Serialize *serialize, const file::MaterialData::Serialize &deserialize) {
    // メモリ内容が合致することを判定する
    assert(memcmp(&serialize->meta, &deserialize.meta, sizeof(serialize->meta)) == 0);
    assert(serialize->materials.size() == deserialize.materials.size());
    assert(serialize->textures.size() == deserialize.textures.size());

    const uint32_t materialsBytes = sizeof(file::MaterialData::Material) * deserialize.materials.size();
    const uint32_t texturesBytes = sizeof(file::MaterialData::Texture) * deserialize.textures.size();

    assert(memcmp(&serialize->materials[0], &deserialize.materials[0], materialsBytes) == 0);
    assert(memcmp(&serialize->textures[0], &deserialize.textures[0], texturesBytes) == 0);

    return true;
}

inline bool testMaterialSerialize(file::MaterialData::Serialize *serialize) {
    file::MaterialData::Serialize deserialize;

    ByteBuffer buffer = file::MaterialData::serialize(serialize);
    assert(!buffer.empty());
    assert(file::MaterialData::deserialize(&deserialize, buffer.unsafe()));
    assert(equals(serialize, deserialize));

    return true;
}

inline bool equals(const file::BoneData::Serialize *serialize, const file::BoneData::Serialize &deserialize) {
    // メモリ内容が合致することを判定する
    assert(memcmp(&serialize->meta, &deserialize.meta, sizeof(serialize->meta)) == 0);
    assert(serialize->bones.size() == deserialize.bones.size());
    assert(serialize->ikLinks.size() == deserialize.ikLinks.size());


    const uint32_t bonesBytes = sizeof(file::BoneData::Bone) * deserialize.bones.size();
    const uint32_t iklinkBytes = sizeof(file::BoneData::IkLink) * deserialize.ikLinks.size();

    assert(memcmp(util::asPointer(serialize->bones), util::asPointer(deserialize.bones), bonesBytes) == 0);

    if (serialize->ikLinks.size()) {
        assert(memcmp(util::asPointer(serialize->ikLinks), util::asPointer(deserialize.ikLinks), iklinkBytes) == 0);
    } else {
        assert(deserialize.ikLinks.empty());
    }
    return true;
}

inline bool testBoneSerialize(file::BoneData::Serialize *serialize) {
    file::BoneData::Serialize deserialize;
    ByteBuffer buffer = file::BoneData::serialize(serialize);
    assert(!buffer.empty());
    assert(file::BoneData::deserialize(&deserialize, buffer.unsafe()));
    assert(equals(serialize, deserialize));
    return true;
}

inline bool equals(const file::MorphData::Serialize *serialize, const file::MorphData::Serialize &deserialize) {

    // メモリ内容が合致することを判定する
    assert(memcmp(&serialize->meta, &deserialize.meta, sizeof(serialize->meta)) == 0);
    assert(serialize->targets.size() == deserialize.targets.size());
    assert(serialize->positionMorphs.size() == deserialize.positionMorphs.size());
    assert(serialize->uvMorphs.size() == deserialize.uvMorphs.size());


    const uint32_t targetBytes = sizeof(file::MorphData::Target) * deserialize.targets.size();
    const uint32_t posMorphBytes = sizeof(file::MorphData::PositionMorph) * deserialize.positionMorphs.size();
    const uint32_t uvMorphBytes = sizeof(file::MorphData::UvMorph) * deserialize.uvMorphs.size();

    if (serialize->targets.size()) {
        assert(memcmp(util::asPointer(serialize->targets), util::asPointer(deserialize.targets), targetBytes) == 0);
        if (!serialize->uvMorphs.empty()) {
            assert(memcmp(util::asPointer(serialize->positionMorphs), util::asPointer(deserialize.positionMorphs), posMorphBytes) == 0);
        }

        if (!serialize->uvMorphs.empty()) {
            assert(memcmp(util::asPointer(serialize->uvMorphs), util::asPointer(deserialize.uvMorphs), uvMorphBytes) == 0);
        }
    }

    return true;
}

inline bool testMorphSerialize(file::MorphData::Serialize *serialize) {
    file::MorphData::Serialize deserialize;
    ByteBuffer buffer = file::MorphData::serialize(serialize);
    assert(!buffer.empty());
    assert(file::MorphData::deserialize(&deserialize, buffer.unsafe()));
    assert(equals(serialize, deserialize));
    return true;
}

inline bool equals(const file::PhysicsData::Serialize *serialize, const file::PhysicsData::Serialize &deserialize) {
    // メモリ内容が合致することを判定する
    assert(memcmp(&serialize->meta, &deserialize.meta, sizeof(serialize->meta)) == 0);
    assert(serialize->rigidJoints.size() == deserialize.rigidJoints.size());
    assert(serialize->rigidBodies.size() == deserialize.rigidBodies.size());

    const uint32_t bodyBytes = sizeof(file::PhysicsData::RigidBody) * deserialize.rigidBodies.size();
    const uint32_t jointBytes = sizeof(file::PhysicsData::RigidJoint) * deserialize.rigidJoints.size();

    if (bodyBytes) {
        assert(memcmp(util::asPointer(serialize->rigidBodies), util::asPointer(deserialize.rigidBodies), bodyBytes) == 0);
    }
    if (jointBytes) {
        assert(memcmp(util::asPointer(serialize->rigidJoints), util::asPointer(deserialize.rigidJoints), jointBytes) == 0);
    }
    return true;
}

inline bool testPhysicsSerialize(file::PhysicsData::Serialize *serialize) {
    file::PhysicsData::Serialize deserialize;
    ByteBuffer buffer = file::PhysicsData::serialize(serialize);
    assert(!buffer.empty());
    assert(file::PhysicsData::deserialize(&deserialize, buffer.unsafe()));
    assert(equals(serialize, deserialize));
    return true;
}

inline bool testAnimationSerialize(file::AnimationData::Serialize *serialize) {
    ByteBuffer buffer = es::file::AnimationData::serialize(serialize);
    assert(!buffer.empty());

    es::file::AnimationData deserialized;
    assert(es::file::AnimationData::deserialize(&deserialized.animation, buffer.unsafe()));

    assert(
            memcmp(&deserialized.animation.meta,
                   &serialize->meta,
                   sizeof(deserialized.animation.meta))
            == 0
    );
    assert(
            memcmp(util::asPointer(deserialized.animation.boneKeys),
                   util::asPointer(serialize->boneKeys),
                   sizeof(file::AnimationData::BoneKeyFrame) * deserialized.animation.boneKeys.size())
            == 0);
    assert(
            memcmp(util::asPointer(deserialized.animation.linkBones),
                   util::asPointer(serialize->linkBones),
                   sizeof(file::AnimationData::SymbolTimeline) * deserialized.animation.linkBones.size())
            == 0);

    return true;
}

inline bool testModelSerialize(file::SkinMeshModelData *serialize) {
    ByteBuffer buffer = file::SkinMeshModelData::serialize(serialize);
    assert(!buffer.empty());

    file::SkinMeshModelData deserialized;
    assert(file::SkinMeshModelData::deserialize(&deserialized, buffer.unsafe()));
    assert(equals(&serialize->mesh, deserialized.mesh));
    assert(equals(&serialize->material, deserialized.material));
    assert(equals(&serialize->bone, deserialized.bone));
    assert(equals(&serialize->morph, deserialized.morph));
    assert(equals(&serialize->physics, deserialized.physics));

    return true;
}

inline bool testSymbolTableSerialize(file::SymbolTable *serialize) {
    ByteBuffer buffer = serialize->serialize();
    assert(!buffer.empty());

    file::SymbolTable deserialize;
    assert(deserialize.deserialize(buffer.unsafe()));

    ByteBuffer deserializeBuffer = deserialize.serialize();

    assert(buffer.length() == deserializeBuffer.length());
    assert(memcmp(buffer.get(), deserializeBuffer.get(), buffer.length()) == 0);

    return true;
}

}
}