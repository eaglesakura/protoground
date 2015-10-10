#include "Bundle.h"
#include <boost/any.hpp>
#include <boost/unordered_map.hpp>

namespace es {

using boost::any;

class Bundle::Impl {
public:
    enum ValueType_e {
        ValueType_int64,
        ValueType_double,
        ValueType_string,
        ValueType_Bundle,
        ValueType_Object,
        ValueType_Unknown,
    };

    struct value_t {
        any value;
        ValueType_e type = ValueType_Unknown;

        value_t() {
        }

        value_t(any v, ValueType_e t) : value(v), type(t) {
        }

        ~value_t() {

        }
    };

    boost::unordered_map<Hash64, value_t> args;
};

bool Bundle::hasValue(const string &key) const {
    if (!impl) {
        return false;
    }

    return impl->args.find(Hash64::from(key)) != impl->args.end();
}

bool Bundle::isEmpty() const {
    if (!impl || impl->args.empty()) {
        return true;
    } else {
        return false;
    }
}

bool Bundle::getBool(const string &key, const bool defValue) const {
    return getInt64(key, defValue) != 0;
}

bool Bundle::getBool(const HashString &key, const bool defValue) const {
    return getInt64(key, defValue) != 0;
}

int32_t Bundle::getInt32(const string &key, const int32_t defValue) const {
    return (int32_t) getInt64(key, defValue);
}

int32_t Bundle::getInt32(const HashString &key, const int32_t defValue) const {
    return (int32_t) getInt64(key, defValue);
}

int64_t Bundle::getInt64(const string &key, const int64_t defValue) const {
    if (!impl) {
        return defValue;
    }

    auto itr = impl->args.find(Hash64::from(key));
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_int64);
        return boost::any_cast<int64_t>(itr->second.value);
    } else {
        return defValue;
    }
}

int64_t Bundle::getInt64(const HashString &key, const int64_t defValue) const {
    if (!impl) {
        return defValue;
    }
    auto itr = impl->args.find(key.shortHash());
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_int64);
        return boost::any_cast<int64_t>(itr->second.value);
    } else {
        return defValue;
    }
}

float Bundle::getFloat(const string &key, const float defValue) const {
    return (float) getDouble(key, defValue);
}

float Bundle::getFloat(const HashString &key, const float defValue) const {
    return (float) getDouble(key, defValue);
}

double Bundle::getDouble(const string &key, const double defValue) const {
    if (!impl) {
        return defValue;
    }

    auto itr = impl->args.find(Hash64::from(key));
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_double);
        return boost::any_cast<double>(itr->second.value);
    } else {
        return defValue;
    }
}


double Bundle::getDouble(const HashString &key, const double defValue) const {
    if (!impl) {
        return defValue;
    }

    auto itr = impl->args.find(key.shortHash());
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_double);
        return boost::any_cast<double>(itr->second.value);
    } else {
        return defValue;
    }
}

string Bundle::getString(const string &key, const string &defValue) const {
    if (!impl) {
        return defValue;
    }

    auto itr = impl->args.find(Hash64::from(key));
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_string);
        return boost::any_cast<string>(itr->second.value);
    } else {
        return defValue;
    }
}

string Bundle::getString(const HashString &key, const string &defValue) const {
    if (!impl) {
        return defValue;
    }

    auto itr = impl->args.find(key.shortHash());
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_string);
        return boost::any_cast<string>(itr->second.value);
    } else {
        return defValue;
    }
}

std::shared_ptr<Object> Bundle::getObject(const HashString &key) const {
    if (!impl) {
        return sp<Object>();
    }

    auto itr = impl->args.find(key.shortHash());
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_Object);
        return boost::any_cast<sp<Object> >(itr->second.value);
    } else {
        return sp<Object>();
    }
}

Bundle Bundle::getBundle(const string &key) const {
    if (!impl) {
        return Bundle();
    }

    auto itr = impl->args.find(Hash64::from(key));
    if (itr != impl->args.end()) {
        assert(itr->second.type == Impl::ValueType_Bundle);
        return boost::any_cast<Bundle>(itr->second.value);
    } else {
        return Bundle();
    }
}

Bundle &Bundle::putBool(const HashString &key, const bool value) {
    putInt64(key, value ? 1 : 0);
    return *this;
}

Bundle &Bundle::putInt32(const string &key, const int32_t value) {
    putInt64(key, value);
    return *this;
}

Bundle &Bundle::putInt32(const HashString &key, const int32_t value) {
    putInt64(key, value);
    return *this;
}

Bundle &Bundle::putInt64(const string &key, const int64_t value) {
    get()->args.insert(std::make_pair(Hash64::from(key), Impl::value_t(value, Impl::ValueType_int64)));
    return *this;
}

Bundle &Bundle::putInt64(const HashString &key, const int64_t value) {
    get()->args.insert(std::make_pair(key.shortHash(), Impl::value_t(value, Impl::ValueType_int64)));
    return *this;
}

Bundle &Bundle::putFloat(const string &key, const float value) {
    putDouble(key, value);
    return *this;
}

Bundle &Bundle::putFloat(const HashString &key, const float value) {
    putDouble(key, value);
    return *this;
}

Bundle &Bundle::putDouble(const string &key, const double value) {
    get()->args.insert(std::make_pair(Hash64::from(key), Impl::value_t(value, Impl::ValueType_double)));
    return *this;
}

Bundle &Bundle::putDouble(const HashString &key, const double value) {
    get()->args.insert(std::make_pair(key.shortHash(), Impl::value_t(value, Impl::ValueType_double)));
    return *this;
}

Bundle &Bundle::putString(const string &key, const string &value) {
    get()->args.insert(std::make_pair(Hash64::from(key), Impl::value_t(value, Impl::ValueType_string)));
    return *this;
}

Bundle &Bundle::putString(const HashString &key, const string &value) {
    get()->args.insert(std::make_pair(key.shortHash(), Impl::value_t(value, Impl::ValueType_string)));
    return *this;
}

Bundle &Bundle::putBundle(const string &key, const Bundle &value) {
    // 循環することは出来ない
    assert(this != &value);

    get()->args.insert(std::pair<Hash64, Impl::value_t>(Hash64::from(key), Impl::value_t(value, Impl::ValueType_Bundle)));
    return *this;
}

Bundle &Bundle::putObject(const HashString &key, const std::shared_ptr<Object> &value) {
    get()->args.insert(std::pair<Hash64, Impl::value_t>(key.shortHash(), Impl::value_t(value, Impl::ValueType_Object)));
    return *this;
}

sp<Bundle::Impl> Bundle::get() {
    if (!impl) {
        impl.reset(new Bundle::Impl());
    }
    return impl;
}

void Bundle::clear() {
    if (impl) {
        impl->args.clear();
    }
}

void Bundle::clear(const string &key) {
    if (!impl) {
        return;
    }

    auto itr = impl->args.find(Hash64::from(key));
    if (itr != impl->args.end()) {
        impl->args.erase(itr);
    }
}

}