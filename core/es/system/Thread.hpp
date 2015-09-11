#pragma once

#include <thread>
#include <mutex>

namespace es {

typedef std::recursive_mutex mutex;
typedef std::unique_lock<mutex> mutex_lock;

}