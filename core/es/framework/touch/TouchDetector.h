#pragma once

#include "es/protoground.hpp"
#include "es/framework/touch/TouchPoint.h"
#include <vector>

namespace es {

class TouchListener;

class TouchDetector : public Object {
public:
    TouchDetector();

    void addListener(const selection_ptr<TouchListener> &listener);

    void removeListener(const selection_ptr<TouchListener> &listener);

    virtual ~TouchDetector() = default;

    /**
     * タッチされている地点の数を取得する
     */
    uint32_t getTouchPoints() const;

    /**
     * インデックスを指定してポイント取得する
     */
    std::shared_ptr<TouchPoint> getTouchPointFromIndex(uint32_t index) const;

    /**
     * IDを指定してポイントを取得する
     */
    std::shared_ptr<TouchPoint> getTouchPointFromId(const uint64_t id) const;

    /**
     * 毎フレームのアップデートを行う
     */
    void onUpdateFrame(const float deltaTimeSec);

    /**
     * イベントが行われたことを通知する。
     */
    void onTouchEvent(const TouchEvent &event);

private:

    /**
     * タップ情報を完全に排除する
     */
    void onHandlePoint(const uint64_t id);

    /**
     * 削除前の一時退避データ
     * ダブルタップの判定に利用する
     */
    struct DropData {
        sp<TouchPoint> point;
        /**
         * タップしたカウント
         */
        int numTaps = 0;
        /**
         * 最後に指を離してからのカウンタ
         */
        NanoTimer releaseTime;
    };

    /**
     *
     */
    std::vector<sp<TouchPoint> > points;

    /**
     * 削除対象データの一覧
     */
    std::vector<DropData> dropPoints;

    /**
     * 通知するリスナ
     */
    std::vector<selp<TouchListener> > listeners;

    /**
     * 最後にチェックした場所のポイント間の距離
     */
    float pinchDistance = 0;

    /**
     * ロングタップとして認識するミリ秒
     */
    uint16_t longTapMs = 400;

    /**
     * 連続タップとして認識するミリ秒
     */
    uint16_t continuationClickMs = 600;

    /**
     * 1点タッチをしている
     */
    bool isSingleTap() const;

    /**
     * 2点間タップをしている
     */
    bool isMultiTap() const;

    /**
     * マルチタップで両方ドラッグしていたtrue
     */
    bool isMultiDrag() const;

    /**
     * マルチタップの中央を取得する
     */
    vec2 getMultiTapCenter() const;

    /**
     * マルチタッチポイント間の距離を取得する
     */
    float getMultiTapPointDistance() const;
};

}


