#pragma once

#include "es/protoground.hpp"
#include "es/asset/IWriter.hpp"
#include <ostream>

namespace es {

namespace internal {

/**
 * シンプルにローカルファイルへの書き込みをサポートする。
 */
class LocalFileWriter : public virtual Object, public virtual IWriter {
public:
    LocalFileWriter() {
    }

    virtual ~LocalFileWriter() {
        close();
    }

    bool open(const string &path, bool append) {
        if (append) {
            fp = fopen(path.c_str(), "ab");
        } else {
            fp = fopen(path.c_str(), "wb");
        }

        return fp != nullptr;
    }

    virtual bool write(const void *buffer, const unsigned size) override {
        auto writeBlock = fwrite(buffer, size, 1, fp);
        return writeBlock == 1;
    }


    virtual void close() override {
        if (!fp) {
            return;
        }

        fclose(fp);
        fp = nullptr;
    }

private:
    FILE *fp = nullptr;
};

}
}
