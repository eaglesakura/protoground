#include "TouchPoint.h"

namespace es {

TouchPoint::TouchPoint(const uint64_t id, const uint32_t touchIndex, const vec2 &beginPos, const float dragDistance) : id(id), touchIndex(touchIndex), beginPos(beginPos), beforePos(beginPos), dragDistance(dragDistance) {
    touchTimer.start();
}

bool TouchPoint::isTouchFinished() const {
    return touchTimeMs != 0;
}

const nano_time &TouchPoint::getBeginTime() const {
    return touchTimer.getStartTime();
}

uint32_t TouchPoint::getTapTimeMs() const {
    if (isTouchFinished()) {
        return touchTimeMs;
    } else {
        // 経過時間を得る
        return (uint32_t) (touchTimer.elapsedMilliSecond());
    }
}

bool TouchPoint::isLongTap(const uint32_t timeMs) const {
    return getTapTimeMs() > timeMs;
}

bool TouchPoint::isDrag() const {
    const float length = glm::length(beginPos - currentPos);
    return length > dragDistance;
}

float TouchPoint::getMoveDistance() const {
    const float length = glm::length(beginPos - currentPos);
    return length;
}

void TouchPoint::onTouchEvent(const TouchEvent &context) {

    beforePos = currentPos;
    currentPos.x = context.position.x;
    currentPos.y = context.position.y;
    if (getMoveDistance() > dragDistance) {
        draged = true;
    }

    if (context.id == TouchEvent::Action_Up || context.id == TouchEvent::Action_Cancel) {
        // タッチされていた時間を設定
        touchTimeMs = getTouchTimeMs();
    }
}

}