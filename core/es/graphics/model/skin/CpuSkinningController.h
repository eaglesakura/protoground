#pragma once

#include "es/protoground.hpp"
#include "es/graphics/model/pose/BoneController.h"
#include "es/graphics/model/file/ModelData.h"
#include "ISkinningController.hpp"

namespace es {

/**
 * CPUでスキニングを実行する。
 *
 * PCであれば十分な処理速度が出るが、モバイルでは十分な速度を出せない。
 * 検証用として利用できる。
 */
class CpuSkinningController : public virtual Object, public virtual ISkinningController {
public:
    CpuSkinningController();

    void initialize(const std::shared_ptr<file::SkinMeshModelData> model, const std::shared_ptr<BoneController> boneController);

    virtual void compute(const uint32_t flags, const VertexAttribute::Complex &dstComplex, void *dstVertices) const override;


    std::shared_ptr<file::SkinMeshModelData> getModel() const {
        return model;
    }

    std::shared_ptr<BoneController> getBoneController() const {
        return boneController;
    }

    virtual ~CpuSkinningController() = default;

private:
    sp<file::SkinMeshModelData> model;
    sp<BoneController> boneController;
};

}


