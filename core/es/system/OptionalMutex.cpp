#include "OptionalMutex.h"

namespace es {

OptionalMutex::OptionalMutex(bool enabled) : enabled(enabled) {

}

std::shared_ptr<mutex_lock> OptionalMutex::lock() {
    if (!isEnable()) {
        return std::shared_ptr<es::mutex_lock>();
    } else {
        return std::shared_ptr<es::mutex_lock>(new es::mutex_lock(mtx));
    }
}

void OptionalMutex::setEnable(bool set) {
    this->enabled = set;
}

bool OptionalMutex::isEnable() const {
    return enabled;
}
}