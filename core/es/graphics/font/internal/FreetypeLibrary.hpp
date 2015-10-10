#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace es {
namespace internal {

class FreetypeLibrary {
public:
    FT_Library library;

    FreetypeLibrary(const FT_Library newLibrary) : library(newLibrary) {
    }


    virtual ~FreetypeLibrary() {
        if (library) {
            FT_Done_FreeType(library);
            library = nullptr;
        }
    }
};

}
}