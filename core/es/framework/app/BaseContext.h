#pragma once

#include <es/system/task/TaskQueue.h>
#include "IApplication.hpp"

namespace es {

class ApplicationRunner;

class IGraphicsEngine;

class GraphicsResourceFactory;

class IDevice;

class TouchDetector;

struct TouchEvent;

/**
 * アプリの実行時Contextを管理する
 */
class BaseContext : public virtual IApplication {
public:
    enum State_e {
        State_Create,
        State_Initialize,
        State_Pause,
        State_Loop,
        State_Destroy,
    };

    virtual void onAppInitialize() override;

    virtual void onAppPause() override;

    virtual void onAppResume() override;

    virtual void onRequestRendering() override;

    virtual void onAppFrame() override;

    virtual void onAppFinalize() override;

    bool isDestroyed() const;

    /**
     * レンダリング用のデバイスを取得する
     */
    std::shared_ptr<IDevice> getCurrentDevice();

    /**
     * タスクを実行する
     *
     * メインスレッドから呼び出された場合はその場で実行し、それ以外の場合はキューに投げ込む。
     */
    void postMainThreadTask(const TaskQueue::TaskFunction &function);

    /**
     * タスクを投げる
     */
    TaskQueue::TaskHandle postTask(const Hash64 &id, const TaskQueue::TaskFunction &function);

    /**
     * メインスレッドのハッシュを取得する
     */
    const Hash64 &getMainThreadId() const;

    /**
     * このメソットがメインスレッドから呼び出されている場合はtrueを返却する
     */
    bool isMainThread() const;

    /**
     * 親クラスへの参照を取得する
     */
    std::shared_ptr<ApplicationRunner> lockRunner() const;

    /**
     * 自分自身への参照を取得する
     */
    std::shared_ptr<BaseContext> lockSelf() const;


    const std::shared_ptr<IGraphicsEngine> &getGraphicsEngine() const;

    const std::shared_ptr<GraphicsResourceFactory> &getGraphcisResourceFactory() const;


    void setGraphicsEngine(const std::shared_ptr<IGraphicsEngine> &engine);

    /**
     * 経過フレーム数を取得する
     */
    uint64_t getFrameCount() const;

    /**
     * フレームのデルタ時間を取得する
     */
    float getDeltaTimeSec() const;

    /**
     * フレームレートの直近1秒実測値を取得する
     */
    uint32_t getRealFrameRate() const;

    /**
     * タッチ解析クラスを取得する。
     *
     * 解析クラスが生成されていない場合、新規に生成する。
     */
    std::shared_ptr<TouchDetector> getTouchDetector() const;

protected:

    /**
     * 指定したIDのタスクを処理する
     */
    virtual void handleTasks(const Hash64 id);

    /**
     * onAppFrame()からコールされるメイン制御
     */
    virtual void onAppFrame(const uint64_t frame, const float deltaTimeSec) = 0;

    /**
     * ディスプレイタッチされた
     */
    virtual void onDisplayTouch(const TouchEvent &event);

private:
    /**
     * このContextを握っているRunnerへの弱参照
     */
    wp<ApplicationRunner> runnerRef;

    /**
     * 自分自身への弱参照
     */
    wp<BaseContext> selfRef;

    State_e state = State_Create;

    mutable std::shared_ptr<TouchDetector> touchDetector;

    Hash64 mainThreadId;

    /**
     * 特定Threadでの動作などを確定させるタスクリスト
     */
    TaskQueue queue;

    std::shared_ptr<IGraphicsEngine> engine;

    mutable std::shared_ptr<GraphicsResourceFactory> resourceFactory;

    /**
     * フレームレートコントロール
     */
    struct {
        /**
         * 最大フレーム数
         */
        uint32_t maxFrameRate = 60;

        /**
         * 経過時刻の観測
         */
        NanoTimer timer;

        /**
         * delta timeの観測値として制限する値
         */
        float deltaTimeLimitSec = 1.0f;

        /**
         * 経過時刻
         */
        float deltaTime = 1.0f / 60.0f;

        /**
         * フレームレート実測値
         */
        uint32_t realRate = 0;

        uint32_t realRate1sec = 0;

        /**
         * フレームレート実測値をカウントするためのタイマー
         */
        NanoTimer frameCountTimer;

        uint64_t frameCount = 0;
    } frameRate;

    friend class ApplicationRunner;
};

}



