#pragma once

#include "es/protoground.hpp"
#include "es/math/Rect.hpp"
#include "es/graphics/Color.hpp"

namespace es {

class IRenderingSurface;

class IDisplayTransfer2D;

/**
 * カプセル化したステート設定
 *
 * 基本的には 0 clear状態で使用し、任意で書き換える
 */
struct render_state {
    /**
     * キャッシュコントロールするステート情報
     */
    enum Flag_e {
        /**
         * カリング ON / Front
         */
                Flag_Cull_Front = 0x1 << 0,

        /**
         * カリング ON / Back
         */
                Flag_Cull_Back = 0x1 << 1,

        /**
         * 深度テストを有効化
         */
                Flag_DepthTest_Enable = 0x1 << 2,

        /**
         * ステンシルテスト有効化
         */
                Flag_StencilTest_Enable = 0x1 << 3,

        /**
         * 表ポリゴンをデフォルトとは反転する
         */
                Flag_FrontFace_CW = 0x1 << 4,
    };

    enum BlendType_e {
        /**
         * 一般的なαブレンドを行う
         */
                BlendType_Alpha = 0,

        /**
         * 加算ブレンディングを行う
         */
                BlendType_Add,

        /**
         * ブレンドを行わない
         */
                BlendType_None,

        /**
         * 不明なブレンド
         * 初回set時に必ず反応するようにしておく
         */
                BlendType_Unknown,
    };

    /**
     * state flags
     */
    uint32_t flags;


    /**
     * GLBlendType_e
     */
    uint8_t blendType;

    /**
     * 描画するラインの太さ
     * 0の場合、ラインの太さを制御しない。
     */
    uint8_t lineWidth;

    static render_state createDefault2D() {
        render_state result = {0};
        return result;
    }

    static render_state createDefault3D() {
        render_state result = {0};
        result.flags = Flag_DepthTest_Enable;
        return result;
    }
};

/**
 * Threadごとに関連付けられたデバイス
 *
 * ContextとSurfaceのペアとして機能する。
 */
class IDevice : public virtual Object {
public:
    enum ClearFlag_e {
        ClearFlag_Color = 0x1 << 0,
        ClearFlag_Depth = 0x1 << 1,
        ClearFlag_Stancil = 0x1 << 2,

        /**
         * 最適な方法で全てのバッファを塗りつぶす。
         *
         * 例えばモバイルGPUのタイルレンダリングの場合、タイルバッファの解放命令でクリアする。
         */
                ClearFlag_AllBuffer = 0x7F,
    };

    /**
     * サーフェイス状態のステート
     */
    struct surface_state {
        RectI16 scissor;
        RectI16 viewport;
        Color backbuffer;
    };

    /**
     * プラットフォームの状態と同既し、デバイスを使える状態にする
     * 初期は同期済みだが、何らかの原因でアプリがステートの変更等を行った場合に同期を取り戻す。
     */
    virtual void syncPlatform() = 0;

    /**
     * 現在のサーフェイス状態を取得する
     */
    virtual surface_state getCurrentState() const = 0;

    /**
     * 現在のステートを上書きする
     */
    virtual void setSurfaceState(const surface_state &state) = 0;

    /**
     * バッファの塗りつぶしを行う
     */
    virtual void clearBuffer(const uint32_t clearFlags) = 0;

    /**
     * サーフェイスの状態を保存して引数の設定に変更する。
     *
     * デバイスは初期状態を持つ。
     */
    virtual void pushSurfaceState(const surface_state &state) = 0;

    /**
     * サーフェイスの状態を一つ前に戻す
     */
    virtual void popSurfaceState() = 0;

    /**
     * サーフェイスを取得する
     */
    virtual std::shared_ptr<IRenderingSurface> getSurface() const = 0;

    /**
     * 座標変換機を取得する
     */
    virtual std::shared_ptr<IDisplayTransfer2D> getDisplayTransfer2D() const = 0;

    virtual ~IDevice() = default;
};

}