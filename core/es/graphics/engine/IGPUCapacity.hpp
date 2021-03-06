﻿#pragma once

#include "es/protoground.hpp"

namespace es {

class IGPUCapacity : public virtual Object {
public:
    /**
     * 対応している拡張機能
     */
    enum GPUExtension_e {
        /**
         * ETC1に対応している
         * GL_OES_compressed_ETC1_RGB8_texture
         */
                GPUExtension_Texture_ETC1,

        /**
         * PVRTC対応
         * GL_IMG_texture_compression_pvrtc
         */
                GPUExtension_Texture_PVRTC,

        /**
         * S3TC対応
         * GL_EXT_texture_compression_s3tc
         */
                GPUExtension_Texture_S3TC,

        /**
         * BGRAテクスチャ対応
         * GL_EXT_texture_format_BGRA8888
         */
                GPUExtension_Texture_BGRA8888,

        /**
         * external画像
         * GL_OES_EGL_image_external
         * for Android（support SurfaceTexture）
         */
                GPUExtension_OES_EGL_image_external,

        /**
         * 深度テクスチャが利用できる
         * GL_OES_depth_texture
         */
                GPUExtension_Texture_Depth,

        /**
         * タイルレンダリング対応
         * GL_EXT_discard_framebuffer
         * PowerVR(iOS, GalaxyNexus, etc...)
         * Mali(Nexus 10, Galaxy S2, etc...)
         */
                GPUExtension_TileBasedDeferredRendering,

        /**
         * half floatテクスチャが利用できる
         */
                GPUExtension_Texture_HalfFloat,

        /**
         * レンダリングバッファにRGB8/RGBA8を利用する
         * 利用ができなかった場合、RGB565/RGB5_A1に丸められる
         */
                GPUExtension_Renderbuffer_RGB8_RGBA8,

        /**
         * レンダリングバッファの深度ステンシルでD24bit/S8bitのパックが可能
         * 利用ができなかった場合、各々に確保される
         */
                GPUExtension_Renderbuffer_PackedDepth24Stencil8,

        /**
         * レンダリングバッファのD24bitが利用可能
         * 利用ができなかった場合、16bitで確保される
         */
                GPUExtension_Renderbuffer_Depth24,

        /**
         * レンダリングバッファのD32bitが利用可能
         * 利用ができなかった場合、16bitで確保される
         */
                GPUExtension_Renderbuffer_Depth32,

        /**
         * VertexArrayObjectをサポートしている。
         * GLES30 / GL40以降は標準サポート
         */
                GPUExtension_VertexArrayObject,

        /**
         * floatテクスチャが利用できる
         */
                GPUExtension_Texture_Float,
        /**
         * num flags
         */
                GPUExtension_Num,
    };

    /**
     * サポートする代表的な系列機を列挙する
     */
    enum GPUFamily_e {
        /**
         * Mali GPU
         * GalaxyS2, GalaxyS3, Nexus10
         */
                GPUFamily_Mali,

        /**
         * PowerVR GPU
         * iOS, GalaxyNexus
         */
                GPUFamily_PowerVR,

        /**
         * Adreno GPU
         * XperiaGX
         */
                GPUFamily_Adreno,

        /**
         * Tegra GPU
         * OptimusPad, GalaxyTab10.1, Nexus7
         */
                GPUFamily_Tegra,

        /**
         * GeForce Desktop
         * MacBook Pro
         */
                GPUFamily_GeForce,

        /**
         * Intel Desktop
         * MacBook Air
         */
                GPUFamily_IntelHdGraphics,


        /**
         * WorkStation
         */
                GPUFamily_Quadro,

        /**
         * not found...
         */
                GPUFamily_Unknown,
    };

    /**
     * ベンダー名を取得する
     */
    virtual const string getVendor() const = 0;

    /**
     * GPU拡張機能をサポートするかを調べる
     */
    virtual const bool isSupport(const GPUExtension_e extension) const = 0;

    /**
     * GPU系列のチェックを行う
     * 代表的な系列機以外はUnknownになる（エミュレーター等）
     */
    virtual const GPUFamily_e getGPUFamily() const = 0;

    virtual ~IGPUCapacity() = default;
};

}