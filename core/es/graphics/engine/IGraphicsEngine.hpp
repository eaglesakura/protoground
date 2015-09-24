#pragma once

#include "es/protoground.hpp"

namespace es {

class IDevice;

class IGPUCapacity;

class GraphicsResourceFactory;

/**
 * グローバルで共有されるグラフィクスコンテキスト
 *
 * プロセスに対して１つ生成される。
 */
class IGraphicsEngine : public virtual Object {
public:
    enum GraphicsEngine_e {
        GraphicsEngine_OpenGL,
        /* GraphicsEngine_DirectX, */ // not support
        /* GraphicsEngine_Metal, */ // not support
        GraphicsEngine_Unknown,
    };

    /**
     * GPUスペックを取得する
     */
    virtual std::shared_ptr<IGPUCapacity> getCapacity() = 0;

    /**
     * 採用されているレンダリングエンジンを取得する
     */
    virtual const GraphicsEngine_e getEngine() = 0;

    /**
     * 現在のThreadに関連付けられたデバイスを取得する
     *
     * このメソッドはthreadのidを手繰るため、実行時間が多少大きくなる。
     */
    virtual std::shared_ptr<IDevice> getCurrentDevice() = 0;

    /**
     * 現在のThreadに関連付けられたデバイスを削除する
     */
    virtual void releaseCurrentDevice() = 0;

    /**
     * デフォルトのリソース生成用ファクトリを取得する
     */
    virtual std::shared_ptr<GraphicsResourceFactory> newGraphicsResourceFactory() = 0;

    virtual ~IGraphicsEngine() = default;
};

}


