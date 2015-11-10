#include <es/math/Math.hpp>
#include "AnimationController.h"
#include "es/graphics/model/file/AnimationData.h"

namespace es {

AnimationController::AnimationController() {

}

void AnimationController::setDataFrameRate(float dataFrameRate) {
    AnimationController::dataFrameRate = dataFrameRate;
}

void AnimationController::setRange(float start, float end) {
    assert(end >= start);

    this->startFrame = start;
    this->endFrame = end;
    this->currentFrame = start;
}

void AnimationController::setRange(const file::AnimationData::Slice& slice) {
    setRange(slice.startFrame, slice.startFrame + slice.numFrame);
}

void AnimationController::reset(std::shared_ptr<file::AnimationData> animation, AnimationController::TermType_e term) {
    assert(animation);

    this->termType = term;
    this->startFrame = 0;
    this->endFrame = animation->animation.meta.endFrame;
    this->currentFrame = 0;
    this->loopCount = 0;
}

void AnimationController::setFrame(float frame) {
    currentFrame = frame;
    normalizeFrame();
}

bool AnimationController::update(float deltaTimeSec) {

    // 指定時刻を経過させる
    {
        const float defDelta = 1.0f / dataFrameRate;
        const float offset = (deltaTimeSec / defDelta);

        currentFrame += offset;
    }
    return normalizeFrame();
}

uint32_t  AnimationController::getLoopCount() const {
    return loopCount;
}

float AnimationController::getCurrentFrame() const {
    return currentFrame;
}

bool AnimationController::normalizeFrame() {
    bool result = false;
    // 開始と終了が同じフレームなら、フレームを固定する
    if (startFrame == endFrame) {
        currentFrame = startFrame;
        ++currentFrame;
        return true;
    }

    // 終端到達してたら、終端処理を行う
    if (currentFrame < startFrame || currentFrame > endFrame) {
        result = true;

        if (termType == TermType_Loop) {
            const float range = (endFrame - startFrame);
            assert(range > 0);

            // 終端ループを行う
            while (currentFrame < startFrame) {
                currentFrame += range;
            }
            while (currentFrame > endFrame) {
                currentFrame -= range;
            }
        } else {
            // 終端停止を行う
            currentFrame = es::minmax(startFrame, endFrame, currentFrame);
        }

        ++loopCount;
    }
    return result;
}
}