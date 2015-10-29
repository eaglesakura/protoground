//
// Created by @eaglesakura on 15/05/20.
//

#ifndef ASSEMBLE_AVERAGECHECKER_HPP
#define ASSEMBLE_AVERAGECHECKER_HPP

#include "es/protoground.hpp"

namespace es {

/**
 * 平均値を計測する
 *
 * 指定した最大値を超える数の平均は出さないようにする。
 */
template<typename T>
class AverageChecker {
    unsigned maxAverageCount = 0;

    /**
     * 指定回数データを捨てる
     *
     * 起動直後等、動作が不安定になりがちな場合に捨てさせる。
     */
    unsigned dropCount = 0;

    /**
     * 現在の合計値
     */
    T sumValue = (T) 0;

    /**
     * 合計回数
     */
    unsigned sumCount = 0;
public:
    AverageChecker() { }

    ~AverageChecker() { }

    void reset() {
        sumCount = 0;
        sumValue = (T) 0;
    }


    /**
     * 値を合計する
     */
    void add(const T &value) {
        if (dropCount > 0) {
            --dropCount;
            return;
        }

        if (maxAverageCount > 0 && sumCount >= maxAverageCount) {
            return;
        }

        sumValue += value;
        ++sumCount;
    }

    /**
     * 平均値を算出する
     */
    T getAverage() const {
        if (sumCount == 0) {
            return (T) 0;
        }

        return sumValue / sumCount;
    }

    /**
     * 最大合計数を指定する
     */
    void setMaxAverageCount(unsigned maxAverageCount) {
        AverageChecker::maxAverageCount = maxAverageCount;
    }


    void setDropCount(unsigned dropCount) {
        AverageChecker::dropCount = dropCount;
    }

    int getMaxAverageCount() const {
        return maxAverageCount;
    }

    T getSumValue() const {
        return sumValue;
    }

    unsigned getSumCount() const {
        return sumCount;
    }
};
}

#endif //ASSEMBLE_AVERAGECHECKER_HPP
