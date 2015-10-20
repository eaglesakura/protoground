#pragma once

#include "es/math/protoground-glm.hpp"

namespace es {

namespace cpu_math {

/**
 * 平均を計算する
 */
inline float average(const vec3 &v) {
    return (v.x + v.y + v.z) / 3.0f;
}

/**
 * vec3メモリに強制変換する
 */
inline const vec3 &asVec3(const vec4 &v) {
    return *((vec3 *) (&v));
}

/**
 * 行列を生成する
 *
 * 主にBullet互換で利用する
 */
inline mat4 createMatrix(const vec3 &translate, const vec3 &eulerXYZrad) {
    return glm::translate(translate) * glm::eulerAngleXYZ(eulerXYZrad.x, eulerXYZrad.y, eulerXYZrad.z);
}

/**
 * src行列のスカラー積を求め、dstに格納する。
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void setScalarM44(float *__restrict dstMatrix, float *__restrict srcMatrix, const float weight) {
    for (int i = 0; i < 4; ++i) {

        for (int k = 0; k < 4; ++k) {
            dstMatrix[k] = srcMatrix[k] * weight;
        }

        dstMatrix += 4;
        srcMatrix += 4;
    }
}

/**
 * ベクトル加算を行う
 */
inline void addVec4Vec4(float *__restrict srcdstVec4, const float *__restrict addVec4) {
    for (int i = 0; i < 4; ++i) {
        *srcdstVec4 = *srcdstVec4 + *addVec4;

        ++srcdstVec4;
        ++addVec4;
    }
}

/**
 * dstに対してsrc行列のスカラーを加算する
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void addScalarM44(float *__restrict dstMatrix, float *__restrict srcMatrix, const float weight) {
    for (int i = 0; i < 4; ++i) {
        dstMatrix[0] += srcMatrix[0] * weight;
        dstMatrix[1] += srcMatrix[1] * weight;
        dstMatrix[2] += srcMatrix[2] * weight;
        dstMatrix[3] += srcMatrix[3] * weight;

        dstMatrix += 4;
        srcMatrix += 4;
    }
}

/**
 * srcVec4とsrcMatrix4x4を乗算してdstに格納する
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void multMat44Vec4(float *__restrict dstVec4, const float *__restrict srcVec4, const float *__restrict srcMatrix4x4) {
    // 最適化されるため、こちらのほうが10%前後高速に動作する
    for (int i = 0; i < 4; ++i) {
        dstVec4[i] = 0;
    }
    for (int i = 0; i < 4; ++i) {
        for (int k = 0; k < 4; ++k) {
            dstVec4[k] += srcMatrix4x4[i * 4 + k] * srcVec4[i];
        }
    }
}

/**
 * srcVec4とsrcMatrix4x4を乗算してdstに格納する
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void multMat43Vec4(float *__restrict dstVec4, const float *__restrict srcVec4, const float *__restrict srcMatrix4x3) {
    for (int i = 0; i < 3; ++i) {
        *dstVec4 = (srcVec4[0] * srcMatrix4x3[0]) +
                   (srcVec4[1] * srcMatrix4x3[1]) +
                   (srcVec4[2] * srcMatrix4x3[2]) +
                   (srcVec4[3] * srcMatrix4x3[3]);

        ++dstVec4;
        srcMatrix4x3 += 4;
    }
}

/**
 * srcVec3とsrcMatrix4x4を乗算してdstに格納する
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void multMat43Vec3(float *__restrict dstVec3, const float *__restrict srcVec4, const float *__restrict srcMatrix4x3) {
    for (int i = 0; i < 3; ++i) {
        *dstVec3 = (srcVec4[0] * srcMatrix4x3[0]) +
                   (srcVec4[1] * srcMatrix4x3[1]) +
                   (srcVec4[2] * srcMatrix4x3[2]) +
                   (srcMatrix4x3[3]);

        ++dstVec3;
        srcMatrix4x3 += 4;
    }
}


#if 0
/**
 * srcVec3とsrcMatrix4x4を乗算してdstに格納する
 * 法線計算用のため、計算内容が他と異なる点に注意。
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void multMat44Vec4Normal(float *__restrict dstVec3, const float *__restrict srcVec3, const float *__restrict srcMatrix4x4) {
    for (int i = 0; i < 3; ++i) {
        dstVec3[i] =
                (srcMatrix4x4[0 * 4 + i] * srcVec3[0]) +
                (srcMatrix4x4[1 * 4 + i] * srcVec3[1]) +
                (srcMatrix4x4[2 * 4 + i] * srcVec3[2]);
    }
}
#endif

/**
 * srcVec3とsrcMatrix4x4を乗算してdstに格納する
 * 法線計算用のため、計算内容が他と異なる点に注意。
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void multMat43Vec4Normal(float *__restrict dstVec3, const float *__restrict srcVec3, const float *__restrict srcMatrix4x3) {
    for (int i = 0; i < 3; ++i) {
        *dstVec3 = (srcVec3[0] * srcMatrix4x3[0]) +
                   (srcVec3[1] * srcMatrix4x3[1]) +
                   (srcVec3[2] * srcMatrix4x3[2]);

        ++dstVec3;
        srcMatrix4x3 += 4;
    }
}


/**
 * 行列の乗算を行う。
 * ret4x3 = after4x3 * before4x3
 * を計算する。
 * before4x3が先に適用され、after4x3が後に適用された状態となる。
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void multMat43Mat43(float *__restrict ret4x3, const float *__restrict after4x3, const float *__restrict before4x3) {
    for (int i = 0; i < 3; ++i) {
        ret4x3[0] =
                before4x3[0] * after4x3[0 * 4 + 0]
                + before4x3[1] * after4x3[1 * 4 + 0]
                + before4x3[2] * after4x3[2 * 4 + 0]
            /*+ before4x3[3] * 0 */;

        ret4x3[1] =
                before4x3[0] * after4x3[0 * 4 + 1]
                + before4x3[1] * after4x3[1 * 4 + 1]
                + before4x3[2] * after4x3[2 * 4 + 1]
            /* + before4x3[3] * 0 */;

        ret4x3[2] =
                before4x3[0] * after4x3[0 * 4 + 2]
                + before4x3[1] * after4x3[1 * 4 + 2]
                + before4x3[2] * after4x3[2 * 4 + 2]
            /* + before4x3[3] * 0*/;

        ret4x3[3] =
                before4x3[0] * after4x3[0 * 4 + 3]
                + before4x3[1] * after4x3[1 * 4 + 3]
                + before4x3[2] * after4x3[2 * 4 + 3]
                + before4x3[3] /* * 1 */;

        before4x3 += 4;
        ret4x3 += 4;
    }
}

/**
 * スカラー積の計算結果をresultに格納する。
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void setScalarM43(float *__restrict ret4x3, const float *__restrict a4x3, const float scalar) {
    for (int i = 2; i >= 0; --i) {
        ret4x3[0] = a4x3[0] * scalar;
        ret4x3[1] = a4x3[1] * scalar;
        ret4x3[2] = a4x3[2] * scalar;
        ret4x3[3] = a4x3[3] * scalar;

        ret4x3 += 4;
        a4x3 += 4;
    }
}

/**
 * スカラー積の計算結果をresultに加算する。
 *
 * 将来的な速度向上を見込むため、メモリは16byte境界である必要がある。
 */
inline void addScalarM43(float *__restrict ret4x3, const float *__restrict a4x3, const float scalar) {
    for (int i = 2; i >= 0; --i) {
        ret4x3[0] += a4x3[0] * scalar;
        ret4x3[1] += a4x3[1] * scalar;
        ret4x3[2] += a4x3[2] * scalar;
        ret4x3[3] += a4x3[3] * scalar;

        ret4x3 += 4;
        a4x3 += 4;
    }
}

/**
 * mat4にtranslate成分をセットする
 * W要素は常に0である必要がある。
 */
inline void setMat44Translate(float *__restrict dstMat44, const float *__restrict translateXYZW) {
    dstMat44 += 12;

    for (int i = 0; i < 3; ++i) {
        *dstMat44 = *translateXYZW;

        ++dstMat44;
        ++translateXYZW;
    }
}

/**
 * mat4にmat3成分をセットする。
 */
inline void setMat44Rotate33(float *__restrict dstMat44, const float *__restrict srcMat33) {
    for (int i = 0; i < 3; ++i) {
        dstMat44[0] = srcMat33[0];
        dstMat44[1] = srcMat33[1];
        dstMat44[2] = srcMat33[2];

        dstMat44 += 4;
        srcMat33 += 3;
    }
}

inline void setMat44Rotate33(mat4 *dst, const mat3 &src) {
    setMat44Rotate33((float *) dst, (float *) &src);
}

/**
 * mat4にtranslate成分を加算する
 * W要素は常に0である必要がある。
 */
inline void addMat44Translate(float *__restrict srcdstMat44, const float *__restrict translateXYZW) {
    srcdstMat44 += 12;
    const float xyzw[4] = {srcdstMat44[0], srcdstMat44[1], srcdstMat44[2], srcdstMat44[3]};

    for (int i = 0; i < 3; ++i) {
        *srcdstMat44 = *translateXYZW + xyzw[i];

        ++srcdstMat44;
        ++translateXYZW;
    }
}

/**
 * mat4にtranslate成分を加算する
 * W要素は常に0である必要がある。
 */
inline void addMat44Translate(float *__restrict dstMat44, const float *__restrict srcMat44, const float *__restrict translateXYZW) {
    dstMat44 += 12;
    srcMat44 += 12;

    for (int i = 0; i < 3; ++i) {
        *dstMat44 = *srcMat44 + *translateXYZW;

        ++dstMat44;
        ++srcMat44;
        ++translateXYZW;
    }
}

/**
 * mat4配列をtransform配列に変換する
 */
inline void mat44toTransform(float *__restrict dstTransformArray, const float *__restrict srcMat44Array, const int numMatrices) {
    if (numMatrices == 0) {
        return;
    }

    for (int n = (numMatrices - 1); n >= 0; --n) {
        // 左側3x4部分を4x3に転置コピーする
        for (int i = 2; i >= 0; --i) {
            for (int k = 3; k >= 0; --k) {
                dstTransformArray[i * 4 + k] = srcMat44Array[k * 4 + i];
            }
        }

        dstTransformArray += 12;
        srcMat44Array += 16;
    }
}


}
}