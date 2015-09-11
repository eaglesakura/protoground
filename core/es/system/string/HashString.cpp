#include "HashString.h"
#include "es/system/string/internal/SharedString.hpp"

namespace es {

HashString::HashString(const string &value) {
    ref.reset(new internal::SharedString(value));
}

HashString::HashString() {

}

bool HashString::operator==(const HashString &check) const {
    if (!ref) {
        return false;
    }
    return ref->hash128 == check.ref->hash128;
}

bool HashString::operator!=(const HashString &check) const {
    if (!ref) {
        return false;
    }
    return ref->hash128 != check.ref->hash128;
}

const Hash128 &HashString::hash() const {
    assert(ref);
    return ref->hash128;
}

const Hash64 &HashString::shortHash() const {
    assert(ref);
    return ref->hash64;
}

const string &HashString::get() const {
    assert(ref);
    return ref->value;
}

HashString &HashString::clearValue() {
    static const string null_value;
    ref->value = null_value;
    return *this;
}

bool HashString::enable() const {
    return (bool) ref;
}
}