#pragma once

#include    "es/protoground.hpp"
#include    <cmath>
#include    <cstdlib>
#include    <complex>
#include	<algorithm>

namespace es {

/**
 * 値が_min <= value <= _maxになるように正規化してvalueを返す。
 */
template<class T>
inline const T minmax(const T _min, const T _max, const T value) {
    if (value < _min) {
        return _min;
    }

    if (value > _max) {
        return _max;
    }

    return value;
}

/**
 * 整数nが2の乗数の場合、trueを返す。
 */
template<class T>
inline bool isPowerOfTwo(const T n) {
    if (!n) {
        return true;
    }

    T bit = 1;
    for (int i = 0; i < 32; ++i) {
        if (n == bit) {
            return true;
        }
        bit <<= 1;
    }

    return false;
}

/**
 * サイズが2のn乗になるように修正する
 */
inline unsigned toPowerOfTwo(const unsigned size) {
	unsigned result = 1;

    while (result < size) {
        result <<= 1;
    }

    return result;
}

/**
 * サイズが2のn乗になるように修正する
 */
inline unsigned toPowerOfTwo(const unsigned x, const unsigned y) {
    return std::max(toPowerOfTwo(x), toPowerOfTwo(y));
}

/**
 * 特定値に近づくように値を移動する
 */
template<typename T>
inline T targetMove(const T now, const T target, const T _offset) {
    const T offset = (_offset < 0 ? -_offset : _offset);
    if (::std::abs(target - now) <= offset) {
        return target;
    } else if (target > now) {
        return now + offset;
    } else {
        return now - offset;
    }
}

/**
 * 2つの浮動小数の同一性をそれなりにチェックする。
 * 演算誤差をある程度考慮する。
 */
inline bool equals(const float a, const float b, const float _check) {
    return ::std::abs(a - b) < _check;
}

/**
 * 2つの浮動小数の同一性をそれなりにチェックする。
 * 演算誤差をある程度考慮する。
 */
inline bool equals(const float a, const float b) {
    return ::std::abs(a - b) < 0.000001f;
}

/**
 * 整数演算の同一性チェック
 */
inline bool equals(const int a, const int b) {
    return a == b;
}

/**
 * 整数演算の同一性チェック
 */
inline bool equals(const uint16_t a, const uint16_t b) {
    return a == b;
}

/**
 * 360度系の正規化を行う。
 *
 *
 * @param now
 * @return
 *
 */
template<typename T>
inline T normalizeDegree(T now) {
    while (now < 0) {
        now += 360.0f;
    }

    while (now >= 360) {
        now -= 360;
    }

    return now;
}

/**
 * 円周率
 */
const double PI = 3.141592653589793;

/**
 * centerから見たpositionが何度になるか360度系で返す。
 * １２時の方向が0度で、反時計回りに角度を進ませる。
 * @param center
 * @param position
 * @return
 */
template<typename VectorType>
float getAngleDegree2D(const VectorType &center, const VectorType &position) {
    float result = 0;

    VectorType temp(position.x - center.x, center.y - position.y);
    if (temp.length() == 0) {
        return 0;
    }
    temp.normalize();

    result = (float) (atan2(temp.y, temp.x) / PI);
    result /= 2;
    result -= 0.25f;

    return normalizeDegree(result * 360.0f);
}

/**
 * -180〜180の間に角度を収める
 */
template<typename T>
inline T getHalfRotateDegree(const T f) {
    return f > 180 ? (f - 360) : f;
}

/**
 * 最短ルートを通って回転を行う
 */
template<typename T>
T turnAngleDegree(const T _current, const T _target, const T offset) {
    T current = getHalfRotateDegree(normalizeDegree(_current));
    T target = getHalfRotateDegree(normalizeDegree(_target));

    if (abs(current - target) <= 180) {
        return normalizeDegree(targetMove(current, target, offset));
    } else {
        if (current > target) {
            current -= 360;
        } else {
            current += 360;
        }
        return normalizeDegree(targetMove(current, target, offset));
    }
}

/**
 *
 */
template<typename T>
inline T degree2radian(const T degree) {
    return (degree * PI) / 180;
}

/**
 *
 */
template<typename T>
inline T radian2degree(const T radian) {
    return (radian * 180) / PI;
}

/**
 * 四捨五入を行う
 */
inline const int32_t round(const float value) {
    return (int) (value + 0.5f);
}

/**
 * 四捨五入を行う
 */
inline const int64_t round(const double value) {
    return (int64_t) (value + 0.5);
}

}
