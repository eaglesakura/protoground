#include "FontFace.h"

namespace es {

const i16vec2 &FontFace::getSize() {
    return size;
}

void FontFace::setThreadSafe(bool set) {
    threadSafe = set;
}

}