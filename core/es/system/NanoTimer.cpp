#include "NanoTimer.h"
#include "es/system/Thread.hpp"

namespace es {

void NanoTimer::sleepMs(const uint ms) {
    std::this_thread::sleep_for(
            std::chrono::milliseconds(ms)
    );
}

NanoTimer &NanoTimer::start() {
    startTime = NanoTimer::now();
    return *this;
}

NanoTimer &NanoTimer::end() {
    endTime = NanoTimer::now();
    return *this;
}

double NanoTimer::second() const {
    return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000.0 / 1000.0 / 1000.0;
}

double NanoTimer::milliSecond() const {
    return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000.0 / 1000.0;
}


double NanoTimer::elapsedMilliSecond() const {
    return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(NanoTimer::now() - startTime).count()) / 1000.0 / 1000.0;
}

double NanoTimer::microSecond() const {
    return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000.0;
}

nano_time NanoTimer::now() {
    return std::chrono::high_resolution_clock::now();
}
}
