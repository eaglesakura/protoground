#include "GLGPUCapacity.h"
#include "es/OpenGL.hpp"
#include "es/internal/protoground-internal.hpp"
#include <es/memory/BitFlags.hpp>
#include <vector>
#include "es/util/StringUtil.h"

namespace es {
namespace gl {

class GLGPUCapacity::Impl {
public:
    string renderer;
    string vendor;
    string version;
    string shaderVersion;
    /**
     * テクスチャユニットの最大数
     */
    uint32_t maxTextureUnits = 0;

    /**
     * テクスチャの一辺の最大サイズ
     */
    uint32_t maxTextureSize = 0;

    /**
     * 拡張設定
     */
    BitFlags<GPUExtension_Num> extension_flags;

    /**
     * GPUファミリー名
     */
    GPUFamily_e gpuFamily = GPUFamily_Unknown;

    /**
     * 頂点属性の最大数
     */
    uint32_t maxVertexAttrbs = 0;

    /**
     * 頂点シェーダでの最大Uniformベクトル数
     */
    uint32_t maxUniformVectorsVs = 0;
    /**
     * フラグメントシェーダでの最大Uniformベクトル数
     */
    uint32_t maxUniformVectorsFs = 0;

    uint32_t maxLineWidth = 0;

    uint32_t maxUniformBufferNum = 0;

    uint32_t maxUniformBufferSize = 0;

    GLGPUCapacity::GLVersion_e glVersion = GLGPUCapacity::GLVersion_Unknown;

    inline int toGLCoreVersion(int coreMajor, int coreMinor) {
        return coreMajor * 10000 +
               coreMinor * 1000;
    }

    void initialize() {
        {
            // レンダラ名と系列機チェック
            const char *pRenderer = (const char *) glGetString(GL_RENDERER);
            renderer = pRenderer;

            struct GPUFamilyGroup {
                /**
                 * チェックするGPU名
                 */
                const char *name;

                /**
                 * 登録されるファミリー
                 */
                GPUFamily_e family;
            } gpu_groups[] = {
                    {"PowerVR",  GPUFamily_PowerVR},
                    {"Mali",     GPUFamily_Mali},
                    {"Tegra",    GPUFamily_Tegra},
                    {"Adreno",   GPUFamily_Adreno},
                    {"GeForce",  GPUFamily_GeForce},
                    {"Intel HD", GPUFamily_IntelHdGraphics},
            };

            for (int i = 0; i < (sizeof(gpu_groups) / sizeof(GPUFamilyGroup)); ++i) {
                if (strstr(pRenderer, gpu_groups[i].name)) {
                    // GPUファミリー
                    gpuFamily = gpu_groups[i].family;
                    eslog("GPU = %s(%s)", gpu_groups[i].name, pRenderer);
                    break;
                }
            }

            if (gpuFamily == GPUFamily_Unknown) {
                eslog("GPU = Unknown(%s)", pRenderer);
            }
        }

        vendor = (const char *) glGetString(GL_VENDOR);
        version = (const char *) glGetString(GL_VERSION);
        shaderVersion = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);

        // バージョンチェック
        {
            GLint major = 0;
            GLint minor = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);
            if (glGetError() == GL_NO_ERROR) {
#if defined(BUILD_Android) || defined(BUILD_iOS)
                // ES2.0以上しかサポートしない
                assert(major >= 2);

                if (major == 3 && minor == 1) {
                    glVersion = GLVersion_ES31;
                } else if (major == 3 && minor == 0) {
                    glVersion = GLVersion_ES30;
                } else if (major == 2) {
                    glVersion = GLVersion_ES20;
                }
#else
                if (major == 4 && minor == 1) {
                    glVersion = GLVersion_41;
                }
#endif
            }
        }
// エクステンション一覧を取得する
        std::vector<string> extensions;
        {
            string extension_names;
            if (glVersion >= 300) {
                GLint numExtensions = 0;
                glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
                for (int i = 0; i < numExtensions; ++i) {
                    std::string ext((const char *) glGetStringi(GL_EXTENSIONS, i));
                    if (extension_names.empty()) {
                        extension_names = ext;
                    } else {
                        extension_names += std::string(" ") + ext;
                    }
                    extensions.push_back(ext);
                }
                assert_gl();

            } else {
                const char *pExtensions = (const char *) glGetString(GL_EXTENSIONS);
                util::split(pExtensions, " ", &extensions);
            }

            struct ExtensionFlag {
                /**
                 * エクステンション名
                 */
                const char *name;

                /**
                 * 利用可能になる拡張フラグ
                 */
                const GPUExtension_e extension;
            };

#define EXTENSION_NAME(def) #def

            const ExtensionFlag EXTENSIONS[] = {
                    //  ETC1(Android)
                    {EXTENSION_NAME(GL_OES_compressed_ETC1_RGB8_texture), GPUExtension_Texture_ETC1},
                    //  PVRTC(PowerVR)
                    {EXTENSION_NAME(GL_IMG_texture_compression_pvrtc),    GPUExtension_Texture_PVRTC},
                    //  s3tc texture(Tegra)
                    {"GL_EXT_texture_compression_s3tc",                   GPUExtension_Texture_S3TC},
                    //  s3tc texture(Tegra)
                    {EXTENSION_NAME(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT),    GPUExtension_Texture_S3TC},
                    //  s3tc texture(Tegra)
                    {EXTENSION_NAME(GL_COMPRESSED_RGB_S3TC_DXT1_EXT),     GPUExtension_Texture_S3TC},
                    //   BGRA(Android)
                    {EXTENSION_NAME(GL_EXT_texture_format_BGRA8888),      GPUExtension_Texture_BGRA8888},
                    //  SurfaceTexture(Android)
                    {EXTENSION_NAME(GL_OES_EGL_image_external),           GPUExtension_OES_EGL_image_external},
                    //  深度テクスチャ
                    {EXTENSION_NAME(GL_OES_depth_texture),                GPUExtension_Texture_Depth},
                    //  half float
                    {EXTENSION_NAME(GL_OES_texture_half_float),           GPUExtension_Texture_HalfFloat},
                    //  フレームバッファの無効化
                    {EXTENSION_NAME(GL_EXT_discard_framebuffer),          GPUExtension_TileBasedDeferredRendering},
                    //  レンダリングバッファにRGB/RGBA8が可能
                    {EXTENSION_NAME(GL_OES_rgb8_rgba8),                   GPUExtension_Renderbuffer_RGB8_RGBA8},
                    //  深度ステンシルパックが可能
                    {EXTENSION_NAME(GL_OES_packed_depth_stencil),         GPUExtension_Renderbuffer_PackedDepth24Stencil8},
                    //  深度ステンシルパックが可能
                    {EXTENSION_NAME(GL_OES_depth24),                      GPUExtension_Renderbuffer_Depth24},
                    //  深度ステンシルパックが可能
                    {EXTENSION_NAME(GL_OES_depth32),                      GPUExtension_Renderbuffer_Depth32},
                    //  Floatテクスチャ
                    {EXTENSION_NAME(GL_OES_texture_float),                GPUExtension_Texture_Float},
                    //  Geforce GTX 750M
                    {EXTENSION_NAME(GL_ARB_half_float_pixel),             GPUExtension_Texture_HalfFloat},
                    {EXTENSION_NAME(GL_ARB_texture_float),                GPUExtension_Texture_Float},
                    {EXTENSION_NAME(GL_ARB_depth_texture),                GPUExtension_Texture_Depth},
                    {EXTENSION_NAME(GL_EXT_packed_depth_stencil),         GPUExtension_Renderbuffer_PackedDepth24Stencil8},
                    //
            };

            // 対応している拡張機能を調べる
            for (int i = 0; i < (sizeof(EXTENSIONS) / sizeof(ExtensionFlag)); ++i) {
                // 拡張機能名がヒットした場合、フラグを有効にする
                if (strstr(extension_names.c_str(), EXTENSIONS[i].name)) {
                    // check index
                    extension_flags.enable(EXTENSIONS[i].extension);
                    eslog("supported extension(%s)", EXTENSIONS[i].name);
                }
            }
#undef  EXTENSION_NAME

            if (glVersion >= toGLCoreVersion(3, 0)) {
                // Support VAO
                extension_flags.enable(GPUExtension_VertexArrayObject);
                eslog("supported extension(GPUExtension_VertexArrayObject)");
            }

        }

        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint *) &maxVertexAttrbs);
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint *) &maxTextureUnits);
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *) &maxTextureSize);
        if (pgdGlGetCompatVersion() >= PgdGraphicsOpenGlCompat_ES30) {
            glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, (GLint *) &maxUniformBufferSize);
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, (GLint *) &maxUniformBufferNum);
        }
        {
            float value[] = {0, 0};
            glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, value);
            maxLineWidth = (uint32_t) value[1];
        }
#ifdef GL_MAX_VERTEX_UNIFORM_VECTORS
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, (GLint *) &maxUniformVectorsVs);
#else
        maxUniformVectorsVs = 250;
#endif

#ifdef GL_MAX_FRAGMENT_UNIFORM_VECTORS
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, (GLint *) &maxUniformVectorsFs);
#else
        maxUniformVectorsFs = 250;
#endif

        eslog("------------ GPU ------------");
        {
            eslog("GL_VERSION = %s", version.c_str());
            eslog("GL_SHADING_LANGUAGE_VERSION = %s", shaderVersion.c_str());
            eslog("GL_VENDOR = %s", vendor.c_str());
            eslog("GL_RENDERER = %s", renderer.c_str());

            // エクステンション一覧を出力する
            for (const string &name: extensions) {
                eslog("GL_EXTENSIONS = %s", name.c_str());
            }

            eslog("GL_MAX_VERTEX_ATTRIBS = %d", maxVertexAttrbs);
            eslog("GL_MAX_TEXTURE_IMAGE_UNITS = %d", maxTextureUnits);
            eslog("GL_MAX_TEXTURE_SIZE = %d", maxTextureSize);
            eslog("GL_MAX_VERTEX_UNIFORM_VECTORS = %d", maxUniformVectorsVs);
            eslog("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d", maxUniformVectorsFs);
            eslog("GL_MAX_UNIFORM_BLOCK_SIZE = %d", maxUniformBufferSize);
            eslog("GL_MAX_UNIFORM_BUFFER_BINDINGS = %d", maxUniformBufferNum);
            eslog("MaxLineWidth = %d", maxLineWidth);
        }
        eslog("-----------------------------");
    }
};

GLGPUCapacity::GLGPUCapacity() {
    impl.reset(new GLGPUCapacity::Impl());
    impl->initialize();
}

const string GLGPUCapacity::getVendor() const {
    return impl->vendor;
}

const bool GLGPUCapacity::isSupport(const IGPUCapacity::GPUExtension_e extension) const {
    return impl->extension_flags.isEnable(extension);
}

const IGPUCapacity::GPUFamily_e GLGPUCapacity::getGPUFamily() const {
    return impl->gpuFamily;
}

Object::QueryResult_e GLGPUCapacity::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    PGD_SUPPORT_QUERY(InterfaceId_OpenGL_GPUCapacity, GLGPUCapacity);
    return Object::queryInterface(interfaceId, resultInterfacePtr);
}

const string &GLGPUCapacity::getGlslVersion() const {
    return impl->shaderVersion;
}

const GLGPUCapacity::GLVersion_e GLGPUCapacity::getGLVersion() const {
    return impl->glVersion;
}

uint32_t GLGPUCapacity::getMaxVertexAttributes() const {
    return impl->maxVertexAttrbs;
}

uint32_t GLGPUCapacity::getMaxTextureUnits() const {
    return impl->maxTextureUnits;
}

uint32_t GLGPUCapacity::getMaxTextureSize() const {
    return impl->maxTextureSize;
}

uint32_t GLGPUCapacity::getMaxUniformVectorsVs() const {
    return impl->maxUniformVectorsVs;
}

uint32_t GLGPUCapacity::getMaxUniformVectorsFs() const {
    return impl->maxUniformVectorsFs;
}

const bool GLGPUCapacity::isES() const {
    return (impl->glVersion % 10) == 0;
}

uint32_t GLGPUCapacity::getMaxLineWidth() const {
    return impl->maxLineWidth;
}

uint32_t GLGPUCapacity::getMaxUniformBufferNum() const {
    return impl->maxUniformBufferNum;
}

uint32_t GLGPUCapacity::getMaxUniformBufferSize() const {
    return impl->maxUniformBufferSize;
}
}
}