#pragma once

#include <cassert>

#if defined(ES_BUILD_GTEST_MODE)

#include <exception>
#include <string>
#include "gtest/gtest.h"


#if defined(_NOEXCEPT)
#define ES_NOEXCEPT    _NOEXCEPT
#elif defined(_GLIBCXX_USE_NOEXCEPT)
#define ES_NOEXCEPT     _GLIBCXX_USE_NOEXCEPT
#endif


namespace es {
namespace debug {

class TestAssertException : public std::exception {
    std::string whatText;
public:
    TestAssertException(const char *__file__, const int __line__, const char *__whatText) {
        char temp[10] = "";
        sprintf(temp, "%d", __line__);

        whatText = std::string(__file__) +
                   std::string(" | L ") +
                   std::string(temp) +
                   std::string("\nERROR : ") +
                   std::string(__whatText) +
                   std::string("\n");
    }

    virtual const char *what() const ES_NOEXCEPT {
        return whatText.c_str();
    }
};

}
}

#ifdef  assert
#undef  assert
#endif

/**
 * gtestの実行は強制停止ではなく、例外を投げてgtestに挙動をハンドリングさせる
 */
#define assert(e)  if(!(e)){ throw ::es::debug::TestAssertException(__FILE__, __LINE__, #e); }

#endif