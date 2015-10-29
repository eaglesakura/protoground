#pragma once

#include "es/protoground.hpp"

namespace es {

namespace file {
struct AnimationData;
}

class AnimationController : public virtual Object {
public:

    enum TermType_e {
        TermType_Stop,
        TermType_Loop,
    };

    AnimationController();


    void setDataFrameRate(float dataFrameRate);

    /**
     * ループの範囲を指定する
     */
    void setLoopRange(float start, float end);

    /**
     * フレームをリセットする
     */
    void reset(std::shared_ptr<file::AnimationData> animation, TermType_e term);


    /**
     * 現在のアニメーションフレームを取得する
     */
    float getCurrentFrame() const;

    /**
     * 更新を行う
     */
    bool update(float deltaTimeSec);

    /*
     *
     */
    void setFrame(float frame);

    /**
     * アニメーションがループした回数を取得する
     */
    uint32_t getLoopCount() const;

    virtual ~AnimationController() = default;

private:
    bool normalizeFrame();

    /**
     * アニメーションデータのフレームレート
     */
    float dataFrameRate = 30;

    /**
     * 開始フレーム番号
     */
    float startFrame = 0;

    /**
     * 終了フレーム番号
     */
    float endFrame = 1;

    float currentFrame = 0;

    /**
     * ループ回数
     */
    unsigned loopCount = 0;

    /**
     * ループ回数
     */
    TermType_e termType = TermType_Stop;
};


}

