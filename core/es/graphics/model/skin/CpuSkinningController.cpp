#include "CpuSkinningController.h"
#include "es/math/VectorMath.hpp"

namespace es {

CpuSkinningController::CpuSkinningController() {

}

void CpuSkinningController::initialize(const std::shared_ptr<file::SkinMeshModelData> model, const std::shared_ptr<BoneController> boneController) {
    assert(model);
    assert(boneController);
    this->model = model;
    this->boneController = boneController;
}

#define ES_SKINNING_ACCESS_ARRAY(type_array, ptr, offset, index) ((type_array)(ptr + (offset * index)))

namespace {
static void CpuSkinningController_skinningM34_withNormal(
        const Transform *__restrict pBoneTable,
        const unsigned numVertices,
        const uint8_t *__restrict srcPositions, const unsigned srcPositionsOffset,
        const uint8_t *__restrict srcNormals, const unsigned srcNormalsOffset,
        const uint8_t *__restrict srcBoneIndices, const unsigned srcBoneIndexOffset,
        const uint8_t *__restrict srcBoneWeights, const unsigned srcBoneWeightOffset,
        uint8_t *__restrict dstPositions, const unsigned dstPositionsOffset,
        uint8_t *__restrict dstNormals, const unsigned dstNormalsOffset) {

    StackBuffer<
            sizeof(mat4)
    > buffer;
    float *mat = (float *) &buffer.pop<mat4>();
    const float MULT_WEIGHT = 1.0f / 255.0f;
    unsigned weight = 0;

    for (int i = (numVertices - 1); i >= 0; --i) {
        cpu_math::setScalarM43(
                mat,
                (float *) (pBoneTable + ES_SKINNING_ACCESS_ARRAY(int16_t*, srcBoneIndices, srcBoneIndexOffset, i)[0]),
                ((float) (ES_SKINNING_ACCESS_ARRAY(uint8_t*, srcBoneWeights, srcBoneWeightOffset, i)[0])) * MULT_WEIGHT
        );

        // 頂点ブレンド
        for (int k = (VertexAttribute::MAX_BONE_WEIGHTS - 1); k > 0; --k) {
            weight = (ES_SKINNING_ACCESS_ARRAY(uint8_t*, srcBoneWeights, srcBoneWeightOffset, i)[k]);
            if (weight) {
                cpu_math::addScalarM43(
                        mat,
                        (float *) (pBoneTable + ES_SKINNING_ACCESS_ARRAY(int16_t*, srcBoneIndices, srcBoneIndexOffset, i)[k]),
                        (float) (weight) * MULT_WEIGHT
                );
            }
        }

        cpu_math::multMat43Vec3((float *) (dstPositions + (i * dstPositionsOffset)),
                                ES_SKINNING_ACCESS_ARRAY(float*, srcPositions, srcPositionsOffset, i),
                                mat
        );
        cpu_math::multMat43Vec4Normal((float *) (dstNormals + (i * dstNormalsOffset)),
                                      ES_SKINNING_ACCESS_ARRAY(float*, srcNormals, srcNormalsOffset, i),
                                      mat
        );
    }
}
}

void CpuSkinningController::compute(const uint32_t flags, const VertexAttribute::Complex &dstComplex, void *dstVertices) const {

    const auto boneTable = boneController->getBoneTable34();

    const unsigned numVertices = model->mesh.meta.vertexNum;
    const unsigned srcVertexBytes = model->mesh.meta.vertexComplex.vertexBytes;
    const void *srcPosition = VertexAttribute::getWriteHeader(model->mesh.vertices.get(), VertexAttribute::POSITION_float3, model->mesh.meta.vertexComplex);
    void *dstPosition = VertexAttribute::getWriteHeader(dstVertices, VertexAttribute::POSITION_float3, dstComplex);
    assert(srcPosition && dstPosition);
    assert(dstPosition != srcPosition);

    const void *srcNormalFloat = VertexAttribute::getWriteHeader(model->mesh.vertices.get(), VertexAttribute::NORMAL_float3, model->mesh.meta.vertexComplex);
    void *dstNormalFloat = VertexAttribute::getWriteHeader(dstVertices, VertexAttribute::NORMAL_float3, dstComplex);

    const void *srcBoneWeightIndex = VertexAttribute::getWriteHeader(model->mesh.vertices.get(), VertexAttribute::BONE_INDEX_i4, model->mesh.meta.vertexComplex);
    const void *srcBoneWeightWeight = VertexAttribute::getWriteHeader(model->mesh.vertices.get(), VertexAttribute::BONE_WEIGHT_i4, model->mesh.meta.vertexComplex);
    assert(srcBoneWeightIndex);
    assert(srcBoneWeightWeight);
    assert(srcBoneWeightWeight != srcBoneWeightIndex);

    if (srcNormalFloat && dstNormalFloat) {
        CpuSkinningController_skinningM34_withNormal(
                boneTable.ptr,
                numVertices,
                (uint8_t *) srcPosition, srcVertexBytes,
                (uint8_t *) srcNormalFloat, srcVertexBytes,
                (uint8_t *) srcBoneWeightIndex, srcVertexBytes,
                (uint8_t *) srcBoneWeightWeight, srcVertexBytes,
                (uint8_t *) dstPosition, dstComplex.vertexBytes,
                (uint8_t *) dstNormalFloat, dstComplex.vertexBytes
        );
    } else {
        // TODO normal無しも実装する
        assert(false);
    }

}

}