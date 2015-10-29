﻿#include    "StringUtil.h"
#include <sstream>
#include <es/memory/Buffer.hpp>

// for Windows
#include <cstdarg>
#include <iosfwd>

// for Android
#include <iterator>

namespace es {

namespace util {

string format(const char *fmt, ...) {
    ByteBuffer buffer = Buffer::createZeroBuffer(strlen(fmt) + 256);

    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *) buffer.get(), fmt, ap);
    va_end(ap);
    return std::string((char *) buffer.get());
}

string format(const unsigned workingBufferBytes, const char *fmt, ...) {
    ByteBuffer buffer = Buffer::createZeroBuffer(strlen(fmt) + workingBufferBytes);

    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *) buffer.get(), fmt, ap);
    va_end(ap);
    return std::string((char *) buffer.get());

}

int split(const std::string &origin, const std::string &delim, std::vector<std::string> *result) {
    const size_t oldSize = result->size();

    std::istringstream iss(origin);
    copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(*result));
    return (int)(result->size() - oldSize);
}

const char *getFileExt(const char *path) {
    if (strchr(path, '.')) {
        return strrchr(path, '.') + 1;
    }
    return nullptr;
}

const char *getFileName(const char *path) {
    if (strchr(path, '/')) {
        return strrchr(path, '/') + 1;
    } else if (strchr(path, '\\')) {
        return strrchr(path, '\\') + 1;
    } else {
        return path;
    }
}

int64_t asInteger(const string &value) {
    int64_t result = 0;
    sscanf(value.c_str(), "%lld", &result);
    return result;
}

double asDouble(const string &value) {
    double result = 0;
    sscanf(value.c_str(), "%lf", &result);
    return result;

}

string replace(const string &origin, const string &before, const string &after) {
    unsigned int index = origin.find(before);
    if (index == string::npos) {
        return origin;
    }

    string result = origin;
    result.replace(index, before.length(), after.c_str());

    return result;
}
}

}
