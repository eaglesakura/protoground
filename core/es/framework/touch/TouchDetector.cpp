#include "TouchDetector.h"
#include "TouchListener.hpp"

namespace es {


TouchDetector::TouchDetector() {

}

void TouchDetector::addListener(const selection_ptr<TouchListener> &listener) {
    listeners.push_back(listener);
}

void TouchDetector::removeListener(const selection_ptr<TouchListener> &listener) {
    auto itr = listeners.begin();
    const auto end = listeners.end();

    while (itr != end) {
        if ((*itr) == listener) {
            listeners.erase(itr);
            return;
        }
        ++itr;
    }
}

uint32_t TouchDetector::getTouchPoints() const {
    return points.size();
}


std::shared_ptr<TouchPoint> TouchDetector::getTouchPointFromIndex(uint32_t index) const {
    if (index < (int) points.size()) {
        return points[index];
    }

    return sp<TouchPoint>();
}

std::shared_ptr<TouchPoint> TouchDetector::getTouchPointFromId(const uint64_t id) const {
    for (const auto &pt : points) {
        if (pt->getId() == id) {
            return pt;
        }
    }
    return sp<TouchPoint>();
}


bool TouchDetector::isSingleTap() const {
    return points.size() == 1;
}

bool TouchDetector::isMultiTap() const {
    return points.size() == 2;
}

bool TouchDetector::isMultiDrag() const {
    if (!isMultiTap()) {
        return false;
    }
    return points[0]->isDrag() && points[1]->isDrag();
}

vec2 TouchDetector::getMultiTapCenter() const {
    assert(isMultiTap());

    const vec2 &beginA = points[0]->getBeginPos();
    const vec2 &beginB = points[1]->getBeginPos();
    return (beginA + beginB) / 2.0f;
}

float TouchDetector::getMultiTapPointDistance() const {
    assert(isMultiTap());

    const vec2 &currentA = points[0]->getCurrentPos();
    const vec2 &currentB = points[1]->getCurrentPos();
    const auto diff = (currentA - currentB);
    return glm::length(diff);
}

void TouchDetector::removeTouchPointFromId(const uint64_t id) {
    auto itr = points.begin();
    const auto end = points.end();
    while (itr != end) {
        if ((*itr)->getId() == id) {
            points.erase(itr);
            return;
        }
        ++itr;
    }
}

void TouchDetector::onTouchEvent(const TouchEvent &event) {

    const uint64_t id = event.id; // タッチポイントのID
    const uint32_t type = event.action; // イベントタイプ
    const float dragDistance = 30;
    sp<TouchPoint> _point = getTouchPointFromId(id); // 対象となるIDを取得する

    if (!_point) {
        // ポイントが見つからなかった

        if (getTouchPoints() >= 2 || type != TouchEvent::Action_Down) {
            // 既に規定以上のポイントがタップされているから無視
            return;
        }

        // 最初のポイントが既に動いてる？
        _point.reset(new TouchPoint(id, points.size(), event.position, dragDistance));

        // ポイントを追加する
        points.push_back(_point);

        if (isSingleTap()) {
            // シングルタップ開始
            for (auto &listener : listeners) {
                listener->onTouchBegin(this, (*_point));
            }
        } else {
            // ピンチの開始
            const vec2 center = getMultiTapCenter();
            pinchDistance = getMultiTapPointDistance();

            for (auto &listener : listeners) {
                listener->onPinchBegin(this, center);
            }
        }
    } else {
        // イベントを更新する
        _point->onTouchEvent(event);

        if (TouchEvent::Action_Down == type) {

        } else if (TouchEvent::Action_Move == type) {
            if (isSingleTap()) {
                if (_point->isDrag()) {
                    // ドラッグされた
                    for (auto &listener : listeners) {
                        listener->onDrag(this, *_point);
                    }
                }
            } else if (isMultiTap()) {
                const vec2 center = getMultiTapCenter();
                const float now_distance = getMultiTapPointDistance();
                if (now_distance != pinchDistance) {
                    for (auto &listener : listeners) {
                        listener->onPinchScaling(this, now_distance / pinchDistance, center);
                    }

                    pinchDistance = now_distance;
                }
            }

        } else if (TouchEvent::Action_Up == type) {
            if (isSingleTap()) {
                // シングルタップ
                if (_point->isDraging()) {
                    for (auto &listener : listeners) {
                        listener->onDragEnd(this, (*_point));
                    }
                } else {
                    for (auto &listener : listeners) {
                        listener->onClick(this, (*_point));
                    }
                }
                removeTouchPointFromId(event.id);
            } else {
                const vec2 center = getMultiTapCenter();
                for (auto &listener : listeners) {
                    listener->onPinchEnd(this, center);
                }
                pinchDistance = 0.1f;
                // マルチタップだったら、どっちのポイントも排除する
                points.clear();
            }
        }
    }
}
}