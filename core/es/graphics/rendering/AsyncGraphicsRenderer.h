#pragma once

#include "es/protoground.hpp"
#include <functional>
#include <vector>

namespace es {

class IDevice;

/**
 * レンダリング用スレッドに渡されるコマンドを構築する
 *
 * 非同期描画に利用される。
 */
class AsyncGraphicsRenderer : public virtual Object {
public:

    /**
     * クロージャにより、メインスレッドの状態でレンダリングを行えるようにする。
     *
     * packet(const AsyncGraphicsRenderer *sender, std::shared_ptr<IDevice> device, const uint globalPass, const uint localPass)
     */
    typedef std::function<void(const AsyncGraphicsRenderer *, std::shared_ptr<IDevice>, const uint, const uint)> packet;

    /**
     * レンダリングパケットを登録する
     *
     * 環境設定用のパケット等も行う。
     * レンダラ内部でソートは行わず、登録されたpacketをそのまま逐次実行する。
     */
    virtual void addPacket(const AsyncGraphicsRenderer::packet &packet);

    /**
     * レンダリングまで生存を保証するオブジェクト
     */
    virtual void addSaveObject(const std::shared_ptr<Object> obj);

    /**
     * 管理しているパケットを削除する
     */
    virtual void clear();

    /**
     * レンダリングを行う。
     *
     * レンダーターゲットの指定等は別途外側で行う。
     */
    virtual void rendering(std::shared_ptr<IDevice> device) const = 0;

    AsyncGraphicsRenderer();

    virtual ~AsyncGraphicsRenderer() = default;

protected:
    /**
     * 登録されているパケットリスト
     */
    std::vector<packet> packetList;

    /**
     * レンダリングまで生存を保障すべきオブジェクト
     */
    std::vector<sp<Object> > saveObjects;
};


}
