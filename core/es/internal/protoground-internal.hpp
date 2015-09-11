#pragma once

#include "es/protoground.hpp"
#include "es/internal/log/Log.h"


/**
 * 文字列をハッシュテーブルに入れこむ
 *
 * 定数として使う文字列は使用時にハッシュ化して利用したほうが容量節約になる
 */
#define ES_GLOBAL_HASH(global_string) (HashStringTable::global().get(string(global_string)).clearValue())

/**
 * 文字列ハッシュテーブルを実装する
 * 文字列は自動的に生成される。
 */
#define ES_GLOBAL_HASH_IMPL(className, hashName) const ::es::HashString className::hashName = ES_GLOBAL_HASH(#className "::" #hashName)


/**
 * 処理が重く、普段は外して問題ないassert文を有効化する
 */
#ifdef  ES_HEAVY_ASSERT
#ifndef heavy_assert
#define heavy_assert(e)    assert(e)
#endif
#else
#ifndef heavy_assert
#define heavy_assert(e)
#endif
#endif