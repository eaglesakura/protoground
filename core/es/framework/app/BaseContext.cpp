#include "BaseContext.h"
#include "es/graphics/engine/IGraphicsEngine.hpp"
#include "es/framework/touch/TouchPoint.h"
#include "es/framework/touch/TouchDetector.h"

namespace es {

void BaseContext::onAppInitialize() {
    assert(engine);
    this->mainThreadId = Hash64::fromCurrentThread();
    state = State_Initialize;
}

const Hash64 &BaseContext::getMainThreadId() const {
    return mainThreadId;
}

void BaseContext::onAppPause() {
    state = State_Pause;
}

void BaseContext::onAppResume() {
    state = State_Loop;
}

void BaseContext::onRequestRendering() {

}

void BaseContext::onAppFrame() {
    state = State_Loop;

    // 前のフレームからの経過時刻を見る
    frameRate.deltaTime = (float) frameRate.timer.end().second();
    frameRate.deltaTime = std::min(frameRate.deltaTime, frameRate.deltaTimeLimitSec);

    // 実行時間のチェックを開始
    frameRate.timer.start();
    {
        queue.execute(mainThreadId);
        onAppFrame(frameRate.frameCount, frameRate.deltaTime);
    }
    // フレーム数カウンタを進める
    ++frameRate.frameCount;
    ++frameRate.realRate;
    if (frameRate.frameCountTimer.end().second() > 1.0) {
        // 1秒超えたらタイマーリセット
        frameRate.realRate1sec = frameRate.realRate;
        frameRate.realRate = 0;
        frameRate.frameCountTimer.start();
    }
}

void BaseContext::onAppFinalize() {
    state = State_Destroy;
}

bool BaseContext::isMainThread() const {
    return mainThreadId == Hash64::fromCurrentThread();
}

void BaseContext::postMainThreadTask(const TaskQueue::TaskFunction &function) {
    if (isMainThread()) {
        function(&queue, mainThreadId);
    } else {
        queue.add(mainThreadId, function).await();
    }
}

TaskQueue::TaskHandle BaseContext::postTask(const Hash64 &id, const TaskQueue::TaskFunction &function) {
    return queue.add(id, function);
}

std::shared_ptr<ApplicationRunner> BaseContext::lockRunner() const {
    return runnerRef.lock();
}

std::shared_ptr<IDevice> BaseContext::getCurrentDevice() {
    return engine->getCurrentDevice();
}

bool BaseContext::isDestroyed() const {
    return state == State_Destroy;
}

const std::shared_ptr<IGraphicsEngine> &BaseContext::getGraphicsEngine() const {
    return engine;
}

void BaseContext::setGraphicsEngine(const std::shared_ptr<IGraphicsEngine> &engine) {
    BaseContext::engine = engine;
}

std::shared_ptr<BaseContext> BaseContext::lockSelf() const {
    return selfRef.lock();
}

uint64_t BaseContext::getFrameCount() const {
    return frameRate.frameCount;
}

float BaseContext::getDeltaTimeSec() const {
    return frameRate.deltaTime;
}

uint32_t BaseContext::getRealFrameRate() const {
    return frameRate.realRate1sec;
}

const std::shared_ptr<GraphicsResourceFactory> &BaseContext::getGraphcisResourceFactory() const {
    if (!resourceFactory && engine) {
        resourceFactory = engine->newGraphicsResourceFactory();
    }
    return resourceFactory;
}

void BaseContext::onDisplayTouch(const TouchEvent &ctx) {
    if (!touchDetector) {
        return;
    }

    touchDetector->onTouchEvent(ctx);
}

std::shared_ptr<TouchDetector> BaseContext::getTouchDetector() const {
    if (!touchDetector) {
        touchDetector.reset(new TouchDetector());
    }

    assert(touchDetector);
    return touchDetector;
}

}