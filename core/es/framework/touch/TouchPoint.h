#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include <es/system/NanoTimer.h>

namespace es {

struct TouchEvent {
    /**
     * タッチ位置
     */
    vec2 position;
    enum Action_e {
        Action_Cancel = -1,
        Action_Down = 0,
        Action_Move,
        Action_Up,
    } action;
    uint64_t id;
};

class TouchDetector;


class TouchPoint : public Object {
public:
    /**
     * タッチポイントを生成する
     *
     * @param id タッチポイントの識別ID
     * @param beginPos 開始位置
     * @param dragDistance ドラッグとして扱う距離
     */
    TouchPoint(const uint64_t id, const uint32_t touchIndex, const vec2 &beginPos, const float dragDistance);

    uint64_t getId() const { return id; }

    bool isDraging() const { return draged; }

    /**
     * タッチ開始位置を取得
     */
    const vec2 &getBeginPos() const { return beginPos; }

    /**
     * タッチ開始時刻を取得
     */
    const nano_time &getBeginTime() const;

    const vec2 &getCurrentPos() const { return currentPos; }


    uint32_t getTouchIndex() const { return touchIndex; }

    /**
     * 一段階前の位置を取得する
     */
    const vec2 &getBeforePos() const { return beforePos; }

    uint32_t getTouchTimeMs() const { return touchTimeMs; }

    /**
     * タップ時間をミリ秒単位で取得する
     */
    uint32_t getTapTimeMs() const;

    /**
     * ロングタップされているならtrue
     * @timeMs このミリ秒以上のタップ時間だったらロングタップとして扱う
     */
    bool isLongTap(const uint32_t timeMs) const;

    /**
     * 指定距離以上移動していたらドラッグとして扱う
     */
    bool isDrag() const;

    /**
     * タッチの移動距離を取得する
     */
    float getMoveDistance() const;

    /**
     * タッチが完了していたらtrue
     */
    bool isTouchFinished() const;


    virtual ~TouchPoint() = default;

private:
    /**
     * タッチイベントの反応を行う
     */
    void onTouchEvent(const TouchEvent &context);

    /**
     * タッチ位置のユニークなID。
     * IDは使いまわされる可能性があることに注意する。
     */
    uint64_t id;

    /**
     * 認識された番号
     */
    uint32_t touchIndex;

    /**
     * タッチの開始座標
     */
    vec2 beginPos;

    /**
     * タッチの現在座標
     */
    vec2 currentPos;

    /**
     * 一つ前の位置
     */
    vec2 beforePos;

    /**
     * タッチ開始時刻
     */
    NanoTimer touchTimer;

    /**
     * タッチ時間
     */
    uint32_t touchTimeMs = 0;

    float dragDistance;

    /**
     * ドラッグしたことがあったらtrue
     */
    bool draged = false;

    friend class TouchDetector;
};

}


