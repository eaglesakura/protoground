#pragma once

#include <es/graphics/font/FontCharactor.h>
#include "es/graphics/font/internal/FreetypeLibrary.hpp"
#include "es/graphics/font/FontCharactor.h"
#include "es/graphics/font/FontFace.h"

namespace es {
namespace internal {

class FontCharactorImpl : public FontCharactor {
    bool tofu = false;
public:
    /**
     * Face情報をコピーする。
     *
     */
    FontCharactorImpl(FontFace *parent, const FT_Face face, const wchar_t charactor) {
        const int FREETYPE_FIXED = 64;
        this->charactor = charactor;
        this->bitmapSize = i16vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        this->bitmapOffset = i16vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        this->advance = i16vec2(face->glyph->advance.x / FREETYPE_FIXED, face->glyph->advance.y / FREETYPE_FIXED);

        const int ascent = face->ascender / FREETYPE_FIXED;
        const int descent = face->descender = FREETYPE_FIXED;

        if (bitmapSize.x == 0 || bitmapSize.y == 0) {
            // 正常に読み込めなかった場合は字形を補正して豆腐文字として扱う。
            bitmapSize = parent->getSize();
            if ((char) charactor == ' ') {
                bitmapSize.x /= 4;
            }
            advance = bitmapSize;
            tofu = true;
        } else {
            // 正常に読み込めた場合
            bitmapBearingY = face->glyph->metrics.horiBearingY / FREETYPE_FIXED;
            advance.x = face->glyph->metrics.horiAdvance / FREETYPE_FIXED;
        }

        baseline.y = (int16_t) ((int32_t) (face->height + face->descender) * (int32_t) (face->size->metrics.y_ppem / face->units_per_EM));
        bitmapOffset.y = baseline.y - bitmapBearingY;
        assert(bitmapSize.x > 0);
        assert(bitmapSize.y > 0);
    }

    virtual ~FontCharactorImpl() {
    }

    /**
     * 文字が正常に読み込まれていない（豆腐文字）の場合にtrueを返却する
     */
    virtual bool isTofu() const {
        return tofu;
    }

};

}
}