#pragma once


#include "es/system/string/IStringConverter.h"

#if defined(BUILD_Windows) /* Check Support Platform */
#include "es/memory/SafeArray.hpp"
#include "es/util/Util.h"

#if defined(BUILD_Windows)
#include <Windows.h>
#endif /* Windows Only */

#include <vector>
#include <cuchar>

namespace es {
namespace internal {

    class CucharStringConverter : public IStringConverter {
    public:
        CucharStringConverter() = default;
        virtual ~CucharStringConverter() = default;

        /**
         * UTF8����������C�h������ɕϊ�����
         */
        virtual wide_string toWideString(const string &utf8) override {
            // C func
            std::vector<wide_char> buffer;
            std::mbstate_t state{}; // zero-initialized to initial state
            char32_t c32;
            const char* head = utf8.c_str();
            const char* end = utf8.c_str() + utf8.length();
            int state_len = -1;
            while((state_len = std::mbrtoc32(&c32, head, (size_t)(end - head), &state)) > 0) {
                if(state_len > 0) {
                    buffer.push_back(c32);
                    head += state_len;
                }
            }
            buffer.push_back(0x00); // END
            return wide_string(&(buffer[0]));
        }

        /**
         * ���C�h�������UTF8�ɖ߂�
         */
        virtual string toUtf8String(const wide_string &wide) override {
            // C func
            std::vector<char> buffer;
            std::mbstate_t state{}; // zero-initialized to initial state
            char utf8[4];
            const char32_t* head = wide.c_str();
            int state_len = -1;
            while((state_len = std::c32rtomb(utf8, *head, &state)) > 0) {
                for(int i = 0; i < state_len; ++i) {
                    buffer.push_back(utf8[i]);
                }
            }
            buffer.push_back(0x00); // END
            return string(&(buffer[0]));
        }

    private:

    };

}
}
#endif
