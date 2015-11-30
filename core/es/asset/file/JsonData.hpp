#pragma once

#include    "es/protoground.hpp"
#include    "es/asset/IAsset.hpp"
#include    "picojson.h"


namespace es {

namespace internal {
    struct JsonRef {
        picojson::value root;
    };
}

class Json : public Object {
public:

    enum JsonType_e {
        JsonType_Object,
        JsonType_Array,
    };

    JsonType_e getType() const {
        if (objRef) {
            return JsonType_Object;
        } else {
            return JsonType_Array;
        }
    }

    unsigned getChildCount() const {
        assert(arrayRef);
        return arrayRef->size();
    }

    std::shared_ptr<Json> getChild(const unsigned index) const {
        assert(arrayRef);
        assert(index < arrayRef->size());

        auto &item = (*arrayRef)[index];

        sp<Json> result(new Json());
        result->ref = this->ref;
        if (item.is<picojson::array>()) {
            result->arrayRef = &item.get<picojson::array>();
        } else {
            result->objRef = &item.get<picojson::object>();
        }

        return result;
    }


    std::shared_ptr<Json> getChild(const string &key) const {
        assert(objRef);

        auto itr = objRef->find(key);
        if (itr == objRef->end()) {
            return sp<Json>();
        }

        sp<Json> result(new Json());
        result->ref = this->ref;
        if (itr->second.is<picojson::array>()) {
            result->arrayRef = &itr->second.get<picojson::array>();
        } else {
            result->objRef = &itr->second.get<picojson::object>();
        }

        return result;
    }

    bool get(const string &key, string *value) const {
        assert(objRef);

        auto itr = objRef->find(key);
        if (itr == objRef->end()) {
            return false;
        } else {
            *value = itr->second.get<string>();
            return true;
        }
    }

    bool get(const string &key, double *value) const {
        assert(objRef);

        auto itr = objRef->find(key);
        if (itr == objRef->end()) {
            return false;
        } else {
            *value = itr->second.get<double>();
            return true;
        }
    }

    template<typename value_type>
    bool getAs(const string &key, value_type *value) const {
        double temp;
        if (get(key, &temp)) {
            *value = static_cast<value_type>(temp);
            return true;
        } else {
            return false;
        }
    }

    static std::shared_ptr<Json> parse(std::shared_ptr<IAsset> asset) {
        if (!asset) {
            return sp<Json>();
        } else {
            return parse(util::toString(asset));
        }
    }

    static std::shared_ptr<Json> parse(const string &json) {
        sp<internal::JsonRef> ref(new internal::JsonRef());
        if (!picojson::parse(ref->root, json).empty()) {
            // parse error!!
            return sp<Json>();
        }


        sp<Json> result(new Json());
        result->ref = ref;

        if (ref->root.is<picojson::object>()) {
            result->objRef = &ref->root.get<picojson::object>();
        } else if (ref->root.is<picojson::array>()) {
            result->arrayRef = &ref->root.get < picojson::array>();
        } else {
            assert(false);
        }

        return result;
    }

    virtual ~Json() = default;
private:
    Json() = default;


    sp<internal::JsonRef> ref;

    picojson::object *objRef = nullptr;
    picojson::array *arrayRef = nullptr;
};


}