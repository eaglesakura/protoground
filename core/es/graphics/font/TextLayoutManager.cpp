#include "TextLayoutManager.h"
#include "es/graphics/font/FontCharactor.h"

namespace es {


TextLayoutManager::TextLayoutManager(const std::shared_ptr<FontFace> &face) {
    this->baselinePositionY = (int) (0.8 * face->getSize().y);
    this->nextBaselinePosition.y = baselinePositionY;
    this->offsetLineY = (int) (1.015 * face->getSize().y);
    // デフォルトは1行でサイズ制限を実質的にナシにする
    this->size = i16vec2(0x6FFF, 0x6FFF);
    this->maxLines = 0x6FFF;
    assert(size.x > 0 && size.y > 0);
}

std::shared_ptr<TextLayoutManager::TextItem> TextLayoutManager::add(const std::shared_ptr<FontCharactor> &charactor, const LayoutOption *option) {
    i16vec2 charSize = charactor->getAdvanceSize();
    const i16vec2 bitmapSize = charactor->getBitmapSize();
    const i16vec2 bitmapOffset = charactor->getBitmapOffset();
    charSize.y = charactor->getBitmapSize().y;
    const int charBaselineOffsetX = (charSize.x);

    assert(charSize.x > 0);
    assert(charSize.y > 0);

    const wchar_t newLineCode = L"\n"[0];
    // 改行コードは改行だけして返す
    if (charactor->getCode() == newLineCode) {
        newLine(option);
        return std::shared_ptr<TextLayoutManager::TextItem>();
    }

    if (isLastLine()) {
        // 最終行はフッダを含んで調整
        if ((nextBaselinePosition.x + charBaselineOffsetX + fooderWidth) > size.x) {
            // 行に収まらないので、フッダを挿入して終了

            return std::shared_ptr<TextItem>();
        }
    } else {
        // 最終行以外は通常レイアウト
        if ((nextBaselinePosition.x + charBaselineOffsetX) > size.x) {
            // 行に収まっていないので、改行する
            newLine(option);
        }
    }
    RectI16 fontPos;
    RectI16 bitmapPos;
    {
        // X位置を確定する
        fontPos.left = nextBaselinePosition.x;
        fontPos.right = fontPos.left + charSize.x;

        // Y位置を確定する
        fontPos.top = nextBaselinePosition.y + bitmapOffset.y;
        fontPos.bottom = fontPos.top + bitmapSize.y;

        // Bitmap位置を確定する
        bitmapPos.left = nextBaselinePosition.x + bitmapOffset.x;
        bitmapPos.right = bitmapPos.left + bitmapSize.x;
        bitmapPos.top = fontPos.top;
        bitmapPos.bottom = fontPos.bottom;
    }

    // 次の文字のベースラインを決定する
    nextBaselinePosition.x = nextBaselinePosition.x + charBaselineOffsetX;

    // レイアウトデータを生成する
    sp<TextLayoutManager::TextItem> item(new TextLayoutManager::TextItem());
    item->bitmapArea = bitmapPos;
    item->fontArea = fontPos;
    item->line = currentLine;
    item->charactor = charactor;

    text.push_back(item);

    return item;
}

void TextLayoutManager::addSpace(const uint xPixels) {
    nextBaselinePosition.x += xPixels;
}

bool TextLayoutManager::newLine(const TextLayoutManager::LayoutOption *option) {
    if (isLastLine()) {
        // 最終行は改行できない
        return false;
    }

    nextBaselinePosition.x = 0;
    nextBaselinePosition.y += offsetLineY;
    ++currentLine;

    return true;
}

bool TextLayoutManager::isLastLine() const {
    if (currentLine >= (maxLines - 1)) {
        return true;
    }

    if ((nextBaselinePosition.y + offsetLineY) > size.y) {
        return true;
    }

    return false;
}

void TextLayoutManager::setFooderText(const std::vector<std::shared_ptr<FontCharactor> > charactors) {
    foodefs = charactors;
    fooderWidth = 0;
    for (const std::shared_ptr<FontCharactor> &font : charactors) {
        fooderWidth += font->getAdvanceSize().x;
    }
}

void TextLayoutManager::setLayoutSize(const uint width, const uint height) {
    size = i16vec2(width, height);
}

const i16vec2 &TextLayoutManager::getLayoutSize() const {
    return size;
}

void TextLayoutManager::setMaxLines(const uint lines) {
    this->maxLines = (uint16_t) lines;
}

uint TextLayoutManager::getMaxLines() const {
    return maxLines;
}

const std::shared_ptr<FontCharactor> &TextLayoutManager::TextItem::getCharactor() const {
    return charactor;
}

const RectI16 &TextLayoutManager::TextItem::getFontArea() const {
    return fontArea;
}

const uint TextLayoutManager::TextItem::getLine() const {
    return line;
}

const RectI16 &TextLayoutManager::TextItem::getBitmapArea() const {
    return bitmapArea;
}


const std::vector<std::shared_ptr<TextLayoutManager::TextItem> > &TextLayoutManager::getText() const {
    return text;
}

const i16vec2 TextLayoutManager::getRenderingSize() const {
    i16vec2 result(0, 0);
    for (const auto &item : text) {
        result.x = std::max<int16_t>(result.x, item->getBitmapArea().right);
        result.y = std::max<int16_t>(result.y, item->getBitmapArea().bottom);
    }
    return result;
}

void TextLayoutManager::clearText() {
    text.clear();
    nextBaselinePosition.x = 0;
    nextBaselinePosition.y = baselinePositionY;
    currentLine = 0;
}
}