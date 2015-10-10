#pragma once

#include "es/protoground.hpp"
#include "es/system/Thread.hpp"

namespace es {

/**
 * マルチスレッド対応有無をoptionで指定できる
 *
 * auto lock = OptionalMutex.lock();
 */
class OptionalMutex {
public:
    OptionalMutex(bool enabled = true);

    ~OptionalMutex() = default;

    /**
     * ロックを取得する
     */
    std::shared_ptr<mutex_lock> lock();

    /**
     * マルチスレッドサポートの有無を指定する
     *
     * falseの場合、lock()メソッドは何も返さなくなる
     */
    void setEnable(bool set);


    bool isEnable() const;

private:
    mutex mtx;
    bool enabled = true;
};

}


