#pragma once

#include    "es/protoground.hpp"
#include    "es/math/Math.hpp"

namespace es {

/**
 * 色情報を扱う。
 * 色情報は一貫してRGBAの順番とする。
 * ライブラリ内部でnativeへ変換する。
 */
union Color {
    /**
     * 1要素ごとの情報
     * リトルエンディアン格納のため、並びは反転する
     */
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } tag;

    /**
     * RGBA情報
     */
    uint32_t rgba;

    Color() {
        RGBAi(255, 255, 255, 255);
    }

    Color(const Color &origin) = default;

    inline float rf() const {
        return (float) tag.r / 255.0f;
    }

    inline float gf() const {
        return (float) tag.g / 255.0f;
    }

    inline float bf() const {
        return (float) tag.b / 255.0f;
    }

    inline float af() const {
        return (float) tag.a / 255.0f;
    }

    inline uint8_t r() const {
        return tag.r;
    }

    inline uint8_t g() const {
        return tag.g;
    }

    inline uint8_t b() const {
        return tag.b;
    }

    inline uint8_t a() const {
        return tag.a;
    }

    /**
     * RGBAを0〜255で指定する
     */
    inline void RGBAi(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        tag.r = r;
        tag.g = g;
        tag.b = b;
        tag.a = a;
    }

    /**
     * RGBAを0〜1.0fで指定する
     */
    inline void RGBAf(const float r, const float g, const float b, const float a) {
        tag.r = (uint8_t) (r * 255.0f);
        tag.g = (uint8_t) (g * 255.0f);
        tag.b = (uint8_t) (b * 255.0f);
        tag.a = (uint8_t) (a * 255.0f);
    }

    /**
     * ARGB色に変換する
     */
    inline uint32_t argb() const {
        struct {
            uint8_t a;
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } _argb;
        _argb.a = tag.a;
        _argb.r = tag.r;
        _argb.g = tag.g;
        _argb.b = tag.b;

        return (*((uint32_t *) &_argb));
    }

    /**
     * 色の加算を行う
     */
    inline Color operator+(const Color &col) const {
        const uint8_t R = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.r + (int32_t) col.tag.r);
        const uint8_t G = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.g + (int32_t) col.tag.g);
        const uint8_t B = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.b + (int32_t) col.tag.b);
        const uint8_t A = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.a + (int32_t) col.tag.a);
        return fromRGBAi(R, G, B, A);
    }

    /**
     * 色の加算を行う
     */
    inline Color &operator+=(const Color &col) {
        tag.r = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.r + (int32_t) col.tag.r);
        tag.g = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.g + (int32_t) col.tag.g);
        tag.b = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.b + (int32_t) col.tag.b);
        tag.a = (uint8_t) std::min<uint32_t>(255, (int32_t) tag.a + (int32_t) col.tag.a);
        return *this;
    }

    /**
     * 色の減算を行う
     */
    inline Color operator-(const Color &col) const {
        const uint8_t R = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.r - (int32_t) col.tag.r);
        const uint8_t G = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.g - (int32_t) col.tag.g);
        const uint8_t B = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.b - (int32_t) col.tag.b);
        const uint8_t A = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.a - (int32_t) col.tag.a);
        return fromRGBAi(R, G, B, A);
    }

    /**
     * 色の減算を行う
     */
    inline Color &operator-=(const Color &col) {
        tag.r = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.r - (int32_t) col.tag.r);
        tag.g = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.g - (int32_t) col.tag.g);
        tag.b = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.b - (int32_t) col.tag.b);
        tag.a = (uint8_t) std::max<uint32_t>(0, (int32_t) tag.a - (int32_t) col.tag.a);
        return *this;
    }

    /**
     * 同一ならtrue
     */
    inline bool operator==(const Color &col) const {
        return rgba == col.rgba;
    }

    /**
     * 同一以外ならtrue
     */
    inline bool operator!=(const Color &col) const {
        return rgba != col.rgba;
    }

    inline static Color fromRGBAf(float r, float g, float b, float a) {
        Color result;
        result.RGBAf(r, g, b, a);
        return result;
    }

    inline static Color fromRGBAi(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        Color result;
        result.RGBAi(r, g, b, a);
        return result;
    }

    /**
     * 色計算を行う。
     * dstColor * (1.0 - srcAlpha) + srcColor * srcAlpha
     */
    static Color blendColorSrcAlpha(const Color dstColor, const Color srcColor) {
        const float src_alpha = srcColor.af();
        const float dst_alpha = 1.0f - src_alpha;

        const float R = (dstColor.rf() * dst_alpha) + (srcColor.rf() * src_alpha);
        const float G = (dstColor.gf() * dst_alpha) + (srcColor.gf() * src_alpha);
        const float B = (dstColor.bf() * dst_alpha) + (srcColor.bf() * src_alpha);

        return fromRGBAf(es::minmax<float>(0, 1, R), es::minmax<float>(0, 1, G), es::minmax<float>(0, 1, B), src_alpha);
    }

    /**
     * 色計算を行う。
     * dstColor * (1.0 - srcAlpha) + srcColor * srcAlpha
     */
    static Color blendColorSrcAlpha(const Color dstColor, const Color srcColor, const float srcAlpha) {
        const float dst_alpha = 1.0f - srcAlpha;

        const float R = (dstColor.rf() * dst_alpha) + (srcColor.rf() * srcAlpha);
        const float G = (dstColor.gf() * dst_alpha) + (srcColor.gf() * srcAlpha);
        const float B = (dstColor.bf() * dst_alpha) + (srcColor.bf() * srcAlpha);

        return fromRGBAf(es::minmax<float>(0, 1, R), es::minmax<float>(0, 1, G), es::minmax<float>(0, 1, B), srcAlpha);
    }

    /**
     * 色をブレンドして返す。
     * 遷移は次の通り
     * blend = 0.0f -> rgba0
     * blend = 1.0f -> rgba1
     * @param rgba0
     * @param rgba1
     * @param blend
     * @return
     */
    static Color blendColor(const Color rgba0, const Color rgba1, float blend) {
        Color result;
        blend = es::minmax<float>(0, 1.0f, blend);

        result.tag.r = (uint8_t) ((((float) rgba1.tag.r) * blend) + (((float) rgba0.tag.r) * (1.0f - blend)));
        result.tag.g = (uint8_t) ((((float) rgba1.tag.g) * blend) + (((float) rgba0.tag.g) * (1.0f - blend)));
        result.tag.b = (uint8_t) ((((float) rgba1.tag.b) * blend) + (((float) rgba0.tag.b) * (1.0f - blend)));
        result.tag.a = (uint8_t) ((((float) rgba1.tag.a) * blend) + (((float) rgba0.tag.a) * (1.0f - blend)));
        return result;
    }

    /**
     * 色を移動させて返す。
     * @param rgba0
     * @param rgba1
     * @param blend
     * @return
     */
    static Color moveColor(const Color now, const Color target, const uint8_t offset) {
        Color result;

        result.tag.r = targetMove(now.tag.r, target.tag.r, offset);
        result.tag.g = targetMove(now.tag.g, target.tag.g, offset);
        result.tag.b = targetMove(now.tag.b, target.tag.b, offset);
        result.tag.a = targetMove(now.tag.a, target.tag.a, offset);
        return result;
    }

    /**
     * α値を乗算する
     */
    inline Color multAlpha(const float a) const {
        return Color::fromRGBAi(tag.r, tag.g, tag.b, (uint8_t) ((af() * a) * 255));
    }

    /**
     * 白を生成する
     */
    static Color white() {
        return fromRGBAi(255, 255, 255, 255);
    }

    /**
     * 黒を生成する
     */
    static Color black() {
        return fromRGBAi(0, 0, 0, 255);
    }
};

static_assert(sizeof(Color) == 4, "sizeof(Color) != 4byte");

}
