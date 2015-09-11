#include "GLPrimitiveRenderer.h"
#include <es/graphics/gl/engine/GLDevice.h>
#include <es/asset/IAsset.hpp>

namespace es {
namespace gl {
namespace debug {

GLPrimitiveRenderer::GLPrimitiveRenderer() {
    this->renderingMaxLines = 0xFFFF;
}

void GLPrimitiveRenderer::initialize(sp<IDevice> originDevice, const VertexAttribute::Complex &complex, std::shared_ptr<AssetManager> assets) {
    GLDevice::query glDevice(originDevice);
    this->mesh.reset(new GLMesh(originDevice));

    // 頂点を確保する
    {
        IMesh::AllocOption opt(IMesh::Target_Vertices);
        opt.num = this->renderingMaxLines * 2;
        opt.flags = IMesh::AllocOption::Flag_Usage_Stream;
        opt.onceBytes = complex.vertexBytes;
        mesh->alloc(originDevice, opt);
    }
    // シェーダーを生成する
    {
        auto vsh = assets->load("gl/debug/wire.vsh");
        auto fsh = assets->load("gl/debug/wire.fsh");
        assert((bool) vsh);
        assert((bool) fsh);

        shader = GLShaderProgram::build(glDevice->getCapacity(), util::toString(vsh), util::toString(fsh), GLGPUCapacity::GLSLVersion_100);
        assert((bool) shader);

        uniform.lookProjection.setLocation(shader->getUniformLocation("lookProjection"));
        assert(uniform.lookProjection.valid());

        GLMesh::Attribute position;
        position.location = shader->getAttribLocation("position");
        position.size = 3;
        position.strideBytes = complex.vertexBytes;
        position.normalize = false;
        position.offsetHeader = VertexAttribute::getVertexOffset(VertexAttribute::POSITION_float3, complex);
        position.type = GL_FLOAT;

        GLMesh::Attribute color;
        color.location = shader->getAttribLocation("color");
        color.size = 4;
        color.strideBytes = complex.vertexBytes;
        color.normalize = true;
        color.offsetHeader = VertexAttribute::getVertexOffset(VertexAttribute::COLOR_i4, complex);
        color.type = GL_UNSIGNED_BYTE;

        assert(position.valid());
        assert(color.valid());
        mesh->addAttribute(position);
        mesh->addAttribute(color);
    }

    this->complex = complex;

}

void GLPrimitiveRenderer::setCamera(const mat4 &look, const mat4 &projection) {
    lookProjection = projection * look;
}

void GLPrimitiveRenderer::setCamera(const mat4 &lookProjection) {
    this->lookProjection = lookProjection;
}


void GLPrimitiveRenderer::renderingLines(std::shared_ptr<IDevice> device, const void *lineVertices, const uint numLines) {
    GLDevice::query glDevice(device);
    shader->bind();
    mesh->bind();

    uniform.lookProjection.upload(lookProjection, GL_FALSE);

    int renderNum = numLines;
    uint8_t *read = (uint8_t *) lineVertices;


#if 0
    struct Sample {
        float pos[3];
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } *sample;
    sample = (Sample *) read;
#endif

    render_state state = render_state::createDefault3D();
    state.lineWidth = 8;
    glDevice->setRenderState(state);

    IMesh::LockOption lockOption(IMesh::Target_Vertices, IMesh::LockOption::LockType_WriteOverwrite);

    // 残数が残っているだけレンダリングする
    while (renderNum > 0) {
        const int instanceNum = std::min<int>(renderingMaxLines, renderNum);

        void *vram = mesh->lock(device, lockOption);
        {
            // 指定数の頂点をアップロードする
            memcpy(vram, read, complex.vertexBytes * (instanceNum * 2));
        }
        mesh->unlock(device, vram);

        // レンダリング
        mesh->renderingArray(GL_LINES, 0, (instanceNum * 2));
        read += (complex.vertexBytes * (instanceNum * 2));
        renderNum -= instanceNum;
    }
}

}
}
}