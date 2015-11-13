#include "IProcessContext.h"
#include "es/asset/AssetManager.h"
#include "es/system/string/HashStringTable.h"

namespace es {

IProcessContext::IProcessContext() {
    HashStringTable::global();
    assetManager.reset(new AssetManager());

}

std::shared_ptr<AssetManager> IProcessContext::getAssetManager() const {
    return assetManager;
}

std::shared_ptr<IStringConverter> IProcessContext::getStringConverter() const {
    return stringConverter;
}

IProcessContext::CpuArchitecture_e IProcessContext::getCpuArchitecture() const {

#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
    #if defined(__ARM_PCS_VFP)
      return CpuArchitecture_ARMv7a_NEON_HardFloat;
    #else
      return CpuArchitecture_ARMv7a_NEON;
    #endif
#else
#if defined(__ARM_PCS_VFP)
    return CpuArchitecture_ARMv7a_HardFloat;
#else
    return CpuArchitecture_ARMv7a;
#endif
#endif
#else
    return CpuArchitecture_ARMEABI;
#endif
#elif defined(__i386__)
    return CpuArchitecture_x86;
#elif defined(__x86_64__)
    return CpuArchitecture_x86_64;
#elif defined(__mips64)
    return CpuArchitecture_Mips64;
#elif defined(__mips__)
    return CpuArchitecture_Mips;
#elif defined(__aarch64__)
    return CpuArchitecture_ARM64v8a;
#elif defined(BUILD_Windows)

#if defined(BUILD_64bit)
    return CpuArchitecture_x86_64;
#else
    return CpuArchitecture_x86;
#endif

#else
    return CpuArchitecture_Unknown;
#endif
}

const string IProcessContext::getCpuArchitectureName() const {
    const char *const names[] = {
            "armeabi-v7a",
            "armeabi-v7a (hard-float)",
            "armeabi-v7a/NEON",
            "armeabi-v7a/NEON (hard-float)",
            "armeabi",
            "x86",
            "x86_64",
            "mips",
            "mips64",
            "arm64-v8a",
            "Unknown",
    };

    return string(names[getCpuArchitecture()]);
}
}