#pragma once

#include "estest/protoground-test.hpp"
#include "es/util/StringUtil.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "es/system/string/IStringConverter.h"
#include "es/graphics/font/Freetype.h"
#include "es/graphics/font/FontCharactor.h"

namespace es {
namespace test {

TEST(FreetypeTest, UtilWideStringConvert) {
    const char *jpn_c = u8"日本語あいうえお";
    ASSERT_NE(strlen(jpn_c), 8);

    wide_string wstr = IProcessContext::getInstance()->getStringConverter()->toWideString(std::string(jpn_c));

    int count = 0;
    for (const wchar_t &w : wstr) {
        if (w) {
            ++count;
        }
    }

    eslog("count(%d) len(%d)", count, wstr.length());
    ASSERT_TRUE(wstr.length() == 8);
}

TEST(FreetypeTest, WstringCheck) {
    eslog("sizeof(wchar_t) = %d bytes", sizeof(wide_char));
    const char *jpn_c = u8"日本語あいうえお";
    ASSERT_NE(strlen(jpn_c), 8);

    wide_string str = IProcessContext::getInstance()->getStringConverter()->toWideString(jpn_c);

    ASSERT_TRUE(str.length() == 8);
}

TEST(FreetypeTest, LoadFont) {
    FT_Library library = nullptr;
    FT_Face face = nullptr;

    ASSERT_TRUE(FT_Init_FreeType(&library) == 0);
    ASSERT_TRUE(library != nullptr);

    std::shared_ptr<IAsset> font = IProcessContext::getInstance()->getAssetManager()->load("font/font-jpn.ttf");
    unsafe_array<uint8_t> fontData = font->read(font->available());
    ASSERT_TRUE(FT_New_Memory_Face(library, fontData.ptr, fontData.length, 0, &face) == 0);
    ASSERT_TRUE(face != nullptr);

    ASSERT_TRUE(FT_Set_Pixel_Sizes(face, 128, 128) == 0);

    ASSERT_TRUE(FT_Load_Char(face, (L"A")[0], FT_LOAD_RENDER) == 0);
    eslog("bitmap W(%d) bitmap H(%d) left(%d) bearingY(%d) height(%d)",
          face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.pitch,
          (int) (face->glyph->metrics.horiBearingY / 64), (int) (face->glyph->metrics.height / 64));
    ASSERT_TRUE(face->glyph->bitmap.width > 0);
    ASSERT_TRUE(face->glyph->bitmap.rows > 0);

    ASSERT_TRUE(FT_Load_Char(face, (L"あ")[0], FT_LOAD_RENDER) == 0);
    eslog("bitmap W(%d) bitmap H(%d) left(%d) bearingY(%d) height(%d)",
          face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.pitch,
          (int) (face->glyph->metrics.horiBearingY / 64), (int) (face->glyph->metrics.height / 64));
    ASSERT_TRUE(face->glyph->bitmap.width > 0);
    ASSERT_TRUE(face->glyph->bitmap.rows > 0);

    FT_Done_Face(face);
    FT_Done_FreeType(library);
//    ASSERT_EQ(FT_Set_Char_Size(face, 16, 48, 512, 512), 0);
}

TEST(FreetypeTest, RenderFont) {
    std::shared_ptr<IAsset> font = IProcessContext::getInstance()->getAssetManager()->load("font/font-jpn.ttf");
    ASSERT_TRUE((bool) font);

    std::shared_ptr<Freetype> freetype(new Freetype());
    Freetype::FontLoadOption option;
    std::shared_ptr<FontFace> fontFace = freetype->load(font, &option);
    ASSERT_TRUE((bool) fontFace);

    fontFace->setSize(128, 126);
    std::shared_ptr<FontCharactor> charactor = fontFace->rendering(L"g"[0], selection_ptr<IImageDecodeCallback>());
    ASSERT_TRUE((bool) charactor);

    eslog("code(%x) font(%d x %d) bmp(%d x %d) bearing(%d)",
          charactor->getCode(),
          charactor->getAdvanceSize().x, charactor->getAdvanceSize().y,
          charactor->getBitmapSize().x, charactor->getBitmapSize().y,
          charactor->getBitmapBearingY()
    );
}

}
}