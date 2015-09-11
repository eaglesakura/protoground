#include "MmsFileConverter.h"


namespace es {
namespace mms {

void MmsFileConverter::convert(file::SkinMeshModelData *result, file::SymbolTable *resultSymbolTable, const std::shared_ptr<::mms::Model> model) {
    VertexAttribute attributes[] = {
            VertexAttribute::POSITION_float3,
            VertexAttribute::NORMAL_float3,
            VertexAttribute::UV_float2,
            VertexAttribute::BONE_WEIGHT_i4,
            VertexAttribute::BONE_INDEX_i4,
            VertexAttribute::END,
    };

    convert(result, resultSymbolTable, model, attributes);
}

void MmsFileConverter::convert(file::SkinMeshModelData *result, file::SymbolTable *resultSymbolTable, const std::shared_ptr<::mms::Model> model, const VertexAttribute *attributes) {
    assert(result);
    assert((bool) model);
    assert(attributes);
    file::SymbolTable table;
    file::SymbolTable &symbols = (resultSymbolTable ? *resultSymbolTable : table);

    // メッシュ構築
    {
        std::shared_ptr<::mms::Mesh> mesh = model->getMesh();

        result->mesh.meta.vertexComplex = VertexAttribute::makeComplex(attributes);
        result->mesh.meta.vertexNum = mesh->getVertexCount();
        result->mesh.meta.indexNum = mesh->getIndicesCount();
        result->mesh.meta.indexDataBytes = mesh->getIndexBytes();

        // 頂点生成
        {
            result->mesh.vertices = Buffer::alignNew(result->mesh.meta.vertexComplex.vertexBytes * result->mesh.meta.vertexNum);
            assert((bool) result->mesh.vertices);

            uint8_t *posWrite = (uint8_t *) VertexAttribute::getWriteHeader(result->mesh.vertices.get(), VertexAttribute::POSITION_float3, result->mesh.meta.vertexComplex);
            uint8_t *normalWrite = (uint8_t *) VertexAttribute::getWriteHeader(result->mesh.vertices.get(), VertexAttribute::NORMAL_float3, result->mesh.meta.vertexComplex);
            uint8_t *uvWrite = (uint8_t *) VertexAttribute::getWriteHeader(result->mesh.vertices.get(), VertexAttribute::UV_float2, result->mesh.meta.vertexComplex);
            uint8_t *boneWeightWrite = (uint8_t *) VertexAttribute::getWriteHeader(result->mesh.vertices.get(), VertexAttribute::BONE_WEIGHT_i4, result->mesh.meta.vertexComplex);
            uint8_t *boneIndexWrite = (uint8_t *) VertexAttribute::getWriteHeader(result->mesh.vertices.get(), VertexAttribute::BONE_INDEX_i4, result->mesh.meta.vertexComplex);

            for (int i = 0; i < result->mesh.meta.vertexNum; ++i) {
                const ::mms::MeshVertex *v = mesh->getVertexPointer(i);
                memcpy(posWrite, &v->skin.pos, sizeof(vec3));
                posWrite += result->mesh.meta.vertexComplex.vertexBytes;

                if (normalWrite) {
                    memcpy(normalWrite, &v->skin.normal, sizeof(vec3));
                    normalWrite += result->mesh.meta.vertexComplex.vertexBytes;
                }

                if (uvWrite) {
                    memcpy(uvWrite, &v->skin.uv, sizeof(vec2));
                    uvWrite += result->mesh.meta.vertexComplex.vertexBytes;
                }

                if (boneWeightWrite && boneIndexWrite) {
                    for (int k = 0; k < ::mms::MeshVertex::MAX_BONE; ++k) {
                        if (k < v->bone.cpuComputeType) {
                            boneWeightWrite[k] = (uint8_t) (v->bone.weights[k] * 255.0f);
                        } else {
                            boneWeightWrite[k] = 0;
                        }
                    }
                    memcpy(boneIndexWrite, v->bone.indices, sizeof(v->bone.indices));
                    boneWeightWrite += result->mesh.meta.vertexComplex.vertexBytes;
                    boneIndexWrite += result->mesh.meta.vertexComplex.vertexBytes;
                }
            }
        }

        // インデックス生成
        {
            result->mesh.indices = Buffer::alignNew(result->mesh.meta.indexDataBytes * result->mesh.meta.indexNum);
            assert((bool) result->mesh.indices);
            memcpy(result->mesh.indices.get(), mesh->getIndicesPointer(), result->mesh.meta.indexDataBytes * result->mesh.meta.indexNum);
        }
    }

    // マテリアル構築
    {
        std::shared_ptr<::mms::MaterialCollection> material = model->getMaterialData();

        int32_t maxTextureIndex = -1;
        for (const auto mtl : material->getMaterials()) {
            file::MaterialData::Material sMaterial;

            sMaterial.symbol = symbols.add(mtl->name);
            sMaterial.diffuse = mtl->diffuse.rgba;

            sMaterial.flags = 0;
            if (mtl->renderFlags & ::mms::Material::NoCulling) {
                sMaterial.flags |= file::MaterialData::Material::Flag_NoCulling;
            }
            if (mtl->renderFlags & ::mms::Material::RenderingEdge) {
                sMaterial.flags |= file::MaterialData::Material::Flag_RenderingEdge;
            }

            sMaterial.indicesNum = mtl->indicesCount;
            sMaterial.textureIndex = (mtl->diffuseTexture ? mtl->diffuseTexture->index : -1);
            maxTextureIndex = std::max<int32_t>(sMaterial.textureIndex, maxTextureIndex);

            result->material.materials.push_back(sMaterial);
        }

        // テクスチャ付きのモデルなら、テクスチャを生成する
        if (maxTextureIndex >= 0) {
            util::valloc(&result->material.textures, maxTextureIndex + 1, true);

            for (const auto mtl : material->getMaterials()) {
                if (mtl->diffuseTexture) {
                    auto diffuse = mtl->diffuseTexture;
                    result->material.textures[diffuse->index].symbol = symbols.add(diffuse->name);
                }
            }
        }
    }

    // ボーン構築
    {
        std::shared_ptr<::mms::BoneCollection> bones = model->getBoneData();

        // 全ボーンを構築
        for (const auto &bone : bones->getBones()) {
            file::BoneData::Bone sBone;
            sBone.symbol = symbols.add(bone->self->name);
            sBone.pos = bone->pos;
            sBone.parentBoneIndex = (bone->parent ? bone->parent->index : -1);
            sBone.ikTargetBoneIndex = bone->self->ikTargetBoneIndex;
            sBone.ikLinkOffset = result->bone.ikLinks.size();
            sBone.ikLinkNum = bone->self->ikLinkCollection.size();
            sBone.ikLoopCount = bone->self->ikLoopCount;
            sBone.ikLimitedRadian = bone->self->ikLimitedRadian;
            sBone.reserve0 = sBone.reserve1 = sBone.reserve1 = 0;

            for (const auto link : bone->self->ikLinkCollection) {
                file::BoneData::IkLink sLink = {0};
                sLink.linkBoneIndex = link.linkBoneIndex;
                sLink.maxRadian = link.maxRadian;
                sLink.minRadian = link.minRadian;
                sLink.flags = 0;
                if (link.rotateLimited) {
                    sLink.flags |= file::BoneData::IkLink::Flag_RotateLimited;
                }
                result->bone.ikLinks.push_back(sLink);
            }
            result->bone.bones.push_back(sBone);
        }
    }

    // モーフ構築
    {
        std::shared_ptr<::mms::MorphCollection> morphs = model->getMorphData();
        for (const auto src : morphs->getMorphs()) {
            file::MorphData::Target target;
            if (src->type == ::mms::MorphType_Position) {
                target.type = file::MorphData::Type_Position;
                target.dataOffset = result->morph.positionMorphs.size();
                target.dataNum = src->num;

                ::mms::PositionMorphVertex *vertex = (::mms::PositionMorphVertex *) src->buffer.get();
                for (int i = 0; i < target.dataNum; ++i) {
                    file::MorphData::PositionMorph pos;
                    pos.offset = vertex->offset;
                    pos.index = vertex->index;
                    result->morph.positionMorphs.push_back(pos);

                    ++vertex;
                }

            } else if (src->type == ::mms::MorphType_UV) {
                target.type = file::MorphData::Type_UV;
                target.dataOffset = result->morph.uvMorphs.size();
                target.dataNum = src->num;

                ::mms::UvMorphVertex *vertex = (::mms::UvMorphVertex *) src->buffer.get();
                for (int i = 0; i < target.dataNum; ++i) {
                    file::MorphData::UvMorph pos;
                    pos.offset = vec2(vertex->offset.x, vertex->offset.y);
                    pos.index = vertex->index;
                    result->morph.uvMorphs.push_back(pos);

                    ++vertex;
                }
            } else {
                continue;
            }

            target.symbol = symbols.add(src->name);
            result->morph.targets.push_back(target);
        }
    }

    // 物理構築
    {
        std::shared_ptr<::mms::PhysicsCollection> physics = model->getPhysicsData();

        for (const auto &src : physics->getRigidBodies()) {
            file::PhysicsData::RigidBody sBody;
            sBody.symbol = symbols.add(src->name);
            sBody.linkBoneIndex = src->linkBoneIndex;
            sBody.noCollisionGroups = src->noCollisitionGroups;
            sBody.groupFlags = 0x1 << src->groupNumber;
            sBody.physicsFlags = 0;
            if (src->rigidType == ::mms::RigidType_BoneLinked) {
                sBody.physicsFlags = file::PhysicsData::PhysicsFlag_BoneLink;
            } else if (src->rigidType == ::mms::RigidType_Physics) {
                sBody.physicsFlags = file::PhysicsData::PhysicsFlag_Physics;
            } else {
                sBody.physicsFlags = (file::PhysicsData::PhysicsFlag_Physics | file::PhysicsData::PhysicsFlag_BoneLink);
            }
            sBody.shapeType = (uint8_t) src->shapeType;
            sBody.size = src->size;
            sBody.position = src->position;
            sBody.rotate = src->rotate;
            sBody.mass = src->mass;
            sBody.translateAttenuation = src->translateAttenuation;
            sBody.rotateAttenuation = src->rotateAttenuation;
            sBody.elasticity = src->elasticity;
            sBody.friction = src->friction;

            result->physics.rigidBodies.push_back(sBody);
        }

        for (const auto &src : physics->getJoints()) {
            file::PhysicsData::RigidJoint sJoint;
            sJoint.symbol = symbols.add(src->name);
            sJoint.rigidBodyIndex = src->rigIndex;
            sJoint.connectRigidBodyIndex = src->connectRigIndex;
            sJoint.position = src->position;
            sJoint.rotate = src->rotate;
            sJoint.translateMinLimit = src->translateMinLimit;
            sJoint.translateMaxLimit = src->translateMaxLimit;
            sJoint.rotateMinLimit = src->rotateMinLimit;
            sJoint.rotateMaxLimit = src->rotateMaxLimit;
            sJoint.springTranslate = src->springTranslate;
            sJoint.springRotate = src->springRotate;

            result->physics.rigidJoints.push_back(sJoint);
        }
    }

    // メタ情報を更新
    file::MeshData::calcMeta(&result->mesh);
}

void MmsFileConverter::convert(file::AnimationData *result, file::SymbolTable *resultSymbolTable, const ::mms::MotionData *motion) {
    assert(result);
    assert(motion);

    file::SymbolTable table;
    file::SymbolTable &symbols = (resultSymbolTable ? *resultSymbolTable : table);

    memset(&result->animation.meta, 0x00, sizeof(file::AnimationData::Meta));

    // ボーン情報を構築
    if (!motion->getBoneMotions().empty()) {
        const auto &boneMotions = motion->getBoneMotions();

        for (const auto &src : boneMotions) {
            assert((bool) src);
            const auto &keys = src->getKeys();

            file::AnimationData::SymbolTimeline sTimeline;
            // シンボル情報を生成
            {
                sTimeline.symbol = symbols.add(src->getName());
                sTimeline.maxFrames = src->getMaxFrame();
                sTimeline.offset = result->animation.boneKeys.size();
                sTimeline.num = keys.size();
                result->animation.linkBones.push_back(sTimeline);
            }
            // キーフレームを全てコピーする
            for (const auto &key : keys) {
                file::AnimationData::BoneKeyFrame sKey;
                sKey.frame = key.frame;
                sKey.pos = key.pos;
                sKey.rotate = key.rotate;
                sKey.reserve0 = sKey.reserve1 = 0;
                result->animation.boneKeys.push_back(sKey);
            }
        }
    }
    result->animation.meta.endFrame = motion->getMaxKeyNumber();
}

}
}