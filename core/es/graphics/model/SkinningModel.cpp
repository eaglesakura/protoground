#include "SkinningModel.h"
#include "es/graphics/model/file/ModelData.h"
#include "es/graphics/model/pose/AnimationPose.h"

namespace es {

SkinningModel::SkinningModel() {

}

void SkinningModel::initialize(const std::shared_ptr<IDevice> device,
                               const std::shared_ptr<IMesh> emptyMesh, const uint32_t verticesAllocFlags, const uint32_t indicesAllocFlags,
                               const std::shared_ptr<file::SkinMeshModelData> model, const std::shared_ptr<file::SymbolTable> symbols) {
    this->mesh = emptyMesh;
    this->model = model;
    this->symbols = symbols;

    assert(emptyMesh);
    assert(model);

    {
        const VertexAttribute::Complex &complex = model->mesh.meta.vertexComplex;
        IMesh::AllocOption opt(IMesh::Target_Vertices);
        opt.onceBytes = complex.vertexBytes;
        opt.num = model->mesh.meta.vertexNum;
        opt.flags = verticesAllocFlags;
        mesh->alloc(device, opt);
        this->attribute = complex;
    }
    {
        IMesh::AllocOption opt(IMesh::Target_Indices);
        opt.onceBytes = model->mesh.meta.indexDataBytes;
        opt.num = model->mesh.meta.indexNum;
        opt.flags = indicesAllocFlags;
        mesh->alloc(device, opt);
    }

    if (!model->bone.bones.empty()) {
        boneController.reset(new BoneController());
        boneController->initialize(model);
    }
}

void SkinningModel::release(const uint32_t releaseFlags) {
    if (model) {
        if (releaseFlags & ReleaseFlag_Vertices) {
            model->mesh.vertices.reset();
        }
        if (releaseFlags & ReleaseFlag_Indices) {
            model->mesh.indices.reset();
        }
    }

    if (releaseFlags & ReleaseFlag_Symbols) {
        symbols.reset();
    }
}

void SkinningModel::uploadVertices(std::shared_ptr<IDevice> device, const VertexAttribute *attributes) {
    assert(mesh);
    assert(model);
    assert((bool) model->mesh.vertices);

    const VertexAttribute::Complex &complex = model->mesh.meta.vertexComplex;
    const uint32_t vertexNum = model->mesh.meta.vertexNum;

    IMesh::LockOption opt(IMesh::Target_Vertices, IMesh::LockOption::LockType_WriteOverwrite);
    if (!attributes) {
        void *vram = mesh->lock(device, opt);
        assert(vram);
        memcpy(vram, model->mesh.vertices.get(), complex.vertexBytes * vertexNum);
        mesh->unlock(device, vram);

        this->attribute = model->mesh.meta.vertexComplex;
    } else {
        void *vram = mesh->lock(device, opt);

        const unsigned dataSize = attributes->getBytes();
        while (!attributes->isEnd()) {
            uint8_t *writeHeader = (uint8_t *) VertexAttribute::getWriteHeader(vram, *attributes, complex);
            uint8_t *readHeader = (uint8_t *) VertexAttribute::getWriteHeader(model->mesh.vertices.get(), *attributes, complex);
            if (writeHeader && readHeader && dataSize) {
                memcpy(writeHeader, readHeader, dataSize);

                writeHeader += dataSize;
                readHeader += dataSize;
            }
            ++attributes;
        }

        mesh->unlock(device, vram);

        this->attribute = VertexAttribute::makeComplex(attributes);
    }
}

void SkinningModel::uploadIndices(std::shared_ptr<IDevice> device) {
    IMesh::LockOption opt(IMesh::Target_Indices);
    opt.type = IMesh::LockOption::LockType_WriteOverwrite;

    void *vram = mesh->lock(device, opt);
    memcpy(vram, model->mesh.indices.get(), model->mesh.meta.indexDataBytes * model->mesh.meta.indexNum);
    mesh->unlock(device, vram);
}

const std::shared_ptr<IMesh> &SkinningModel::getMesh() const {
    return mesh;
}

const std::shared_ptr<file::SkinMeshModelData> &SkinningModel::getModel() const {
    return model;
}

const std::shared_ptr<file::SymbolTable> &SkinningModel::getSymbols() const {
    return symbols;
}

const std::shared_ptr<BoneController> &SkinningModel::getBoneController() const {
    return boneController;
}

std::shared_ptr<AnimationPose> SkinningModel::newAnimationPose() const {
    assert(model);

    return sp<AnimationPose>(new AnimationPose(model));
}

const VertexAttribute::Complex SkinningModel::getVertexAttributes() const {
    return attribute;
}

const std::string SkinningModel::getSymbolString(const uint64_t symbol) const {
    if (symbols) {
        return symbols->find(symbol);
    } else {
        return string();
    }
}

bool SkinningModel::hasBones() const {
    if (!model) {
        return false;
    } else {
        return !model->bone.bones.empty();
    }
}

bool SkinningModel::hasPhysics() const {
    if (!model) {
        return false;
    } else {
        return !model->physics.rigidBodies.empty();
    }
}
}