#pragma once

#include    "es/protoground-types.hpp"
#include    "es/debug/Testing.hpp"
#include    <string>
#include    <memory>
#include    <sys/types.h>
#include    "es/system/Object.hpp"
#include    "es/memory/selection_ptr.hpp"

namespace es {

// typedef unsigned int unsigned;

template<typename T>
using sp = ::std::shared_ptr<T>;

template<typename T>
using wp = ::std::weak_ptr<T>;

template<typename T>
using selp  = ::es::selection_ptr<T>;

/**
 * 文字列操作
 */
typedef std::string string;

/**
 * ワイド文字列
 */
typedef std::u32string wide_string;

/**
 * ワイド文字列の内部型
 * システム的に不都合があった場合は変更できるようにする。
 */
typedef char32_t wide_char;

#define PGD_STRING(msg) U##msg

}
