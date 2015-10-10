#pragma once

#include    "es/protoground.hpp"
#include    <chrono>

namespace es {

typedef std::chrono::high_resolution_clock::time_point nano_time;

/**
 * ナノ秒単位の測定を行う高分解能タイマー
 */
class NanoTimer {
public:
    NanoTimer() {
        start();
        end();
    }

    ~NanoTimer() = default;


    NanoTimer &start();

    NanoTimer &end();

    /**
     * 秒単位で取得する
     */
    double second() const;

    /**
     * ミリ秒単位で取得する
     */
    double milliSecond() const;

    /**
     * 開始時刻からの経過時間をミリ秒単位で取得する
     */
    double elapsedMilliSecond() const;

    /**
     * マイクロ秒単位で取得する
     */
    double microSecond() const;

    const nano_time &getStartTime() const { return startTime; }

    const nano_time &getEndTime() const { return endTime; }

    static void sleepMs(const unsigned ms);

    /**
     * 適当な起点からの時刻を取得する。
     * これは差分を取得するための起点時刻として利用するが、システム時刻としては利用できないことに注意する。
     */
    static nano_time now();

    /**
     * システムのミリ秒単位の現在時刻を取得する
     */
    static uint64_t systemMilliSeconds();

private:
    nano_time startTime;
    nano_time endTime;
};
}
