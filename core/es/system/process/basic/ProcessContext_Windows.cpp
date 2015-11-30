#include "ProcessContext.h"

#if defined(BUILD_Windows)
#include "es/internal/protoground-internal.hpp"
#include "Windows.h"

namespace es {


IProcessContext::PlatformType_e ProcessContext::getPlatform() const {
    return PlatformType_Windows;
}

bool ProcessContext::getPlatformVersion(VersionInfo* result) const {
    if (versionCahe.major) {
        // キャッシュを使用する
        *result = versionCahe;
        return true;
    }

    OSVERSIONINFOEX verInfo = { 0 };
    verInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (!GetVersionEx((OSVERSIONINFO*) &verInfo)) {
        // 正常に取得できなかった
        return false;
    }

    if (verInfo.dwMajorVersion <= 6) {
        switch (verInfo.dwMinorVersion) {
            case 0:
            case 1:
                // Win7
                versionCahe.major = 7;
                versionCahe.minor = 0;
                versionCahe.displayName = "Windows 7";
                break;
            case 2:
                // Win8
                versionCahe.major = 8;
                versionCahe.minor = 0;
                versionCahe.displayName = "Windows 8";
                break;
            case 3:
                // Win8.1
                versionCahe.major = 8;
                versionCahe.minor = 1;
                versionCahe.displayName = "Windows 8.1";
                break;
            default:
                // Win10
                versionCahe.major = 10;
                versionCahe.minor = 0;
                versionCahe.displayName = "Windows 10";
                break;
        }
    } else {
        // Windows10 or later
        versionCahe.major = 10;
        versionCahe.minor = 0;
        versionCahe.displayName = "Windows 10 or Later";
    }

    *result = versionCahe;
    return true;
}

}

#endif