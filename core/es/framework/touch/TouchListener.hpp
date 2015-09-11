#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"

namespace es {


class TouchPoint;

class TouchDetector;

/**
 * タッチ操作のコールバックを受け取る。
 */
class TouchListener {
public:

    /**
     * Viewがクリックされた。
     * 指が離された瞬間に認識される
     * onDragEnd()とどちらかが呼び出される。
     */
    virtual void onClick(const TouchDetector *detector, const TouchPoint &point) { }

    /**
     * ドラッグを終了させた。
     * onClick()とどちらかが呼び出される。
     */
    virtual void onDragEnd(const TouchDetector *detector, const TouchPoint &point) { }

    /**
     * タッチが開始された
     */
    virtual void onTouchBegin(const TouchDetector *detector, const TouchPoint &point) { }

    /**
     * Viewがドラッグされた
     */
    virtual void onDrag(const TouchDetector *detector, const TouchPoint &point) { }

    /**
     * マルチタッチが開始された
     */
    virtual void onPinchBegin(const TouchDetector *detector, const vec2 &center) { }

    /**
     * ピンチが変更された
     */
    virtual void onPinchScaling(const TouchDetector *detector, const float scale, const vec2 &center) { }

    /**
     * ピンチ操作が終了された
     */
    virtual void onPinchEnd(const TouchDetector *detector, const vec2 &center) { }

    virtual ~TouchListener() = default;

};


}