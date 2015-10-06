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
     *
     * trueを返した場合、ハンドリングが行われたため、複数回タップ（onContinuationClick)は呼び出されない。
     */
    virtual bool onClick(const TouchDetector *detector, const TouchPoint &point) { return false; }

    /**
     * ロングクリックが行われた。
     * ロングクリックとして処理しないなら、falseを返す。
     * trueを返却した場合、ハンドリングされたとみなす。
     */
    virtual bool onLongClick(const TouchDetector *detector, const TouchPoint &point) { return false; }

    /**
     * 複数回タップ（ダブルタップ等）が行われた。
     *
     * numClicksは必ず2以上（ダブルタップ）となる。
     * falseを返した場合、データをドロップせずに継続を行い、トリプルタップ以上の認識を行える。
     */
    virtual bool onContinuationClick(const TouchDetector *detector, const TouchPoint &point, const uint32_t numClicks) { return true; }

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