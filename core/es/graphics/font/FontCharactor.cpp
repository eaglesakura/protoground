#include "FontCharactor.h"

namespace es {

const i16vec2 &FontCharactor::getBitmapSize() const {
    return bitmapSize;
}

const int16_t FontCharactor::getBitmapBearingY() const {
    return bitmapBearingY;
}

const wchar_t FontCharactor::getCode() const {
    return charactor;
}

const i16vec2 &FontCharactor::getBitmapOffset() const {
    return bitmapOffset;
}

const i16vec2 &FontCharactor::getAdvanceSize() const {
    return advance;
}

}