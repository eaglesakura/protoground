#include "Freetype.h"
#include "es/graphics/font/internal/FontFaceImpl.hpp"
#include "es/graphics/font/internal/FreetypeLibrary.hpp"

namespace es {

using namespace internal;

class Freetype::Impl {
public:
    std::shared_ptr<FreetypeLibrary> library;
};

Freetype::Freetype() {
    impl.reset(new Impl());
    {
        FT_Library library = nullptr;
        int error = FT_Init_FreeType(&library);
        assert(error == 0);

        impl->library.reset(new FreetypeLibrary(library));
    }
}

std::shared_ptr<FontFace> Freetype::load(std::shared_ptr<IAsset> asset, FontLoadOption *option) {
    std::shared_ptr<FontFace> result;

    unsafe_array<uint8_t> buffer = asset->read(asset->available());
    std::vector<uint8_t> copyBuffer(buffer.length);
    memcpy(util::asPointer(copyBuffer), buffer.ptr, buffer.length);

    FT_Face face = nullptr;
    int error = 0;
    if ((error = FT_New_Memory_Face(impl->library->library, buffer.ptr, buffer.length, 0, &face)) != 0) {
        // error
        eslog("FT_New_Memory_Face failed(%d)", error);
        return result;
    }

    result.reset(new internal::FontFaceImpl(impl->library, face, asset));

    return result;
}
}