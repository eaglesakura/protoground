#pragma once

#include "es/protoground.hpp"
#include <functional>

namespace es {

class AssetManager;

class IStringConverter;

/**
 * プロセス情報を管理する
 */
class IProcessContext : public Object {
public:
    typedef std::function<void()> ThreadCallback;

    enum PlatformType_e {
        /**
         * Androidで実行されている
         */
                PlatformType_Android,

        /**
         * Mac OSXで実行されている
         */
                PlatformType_OSX,

        /**
         * Windowsで実行されている
         */
                PlatformType_Windows,
    };

    enum CpuArchitecture_e {
        CpuArchitecture_ARMv7a,
        CpuArchitecture_ARMv7a_HardFloat,
        CpuArchitecture_ARMv7a_NEON,
        CpuArchitecture_ARMv7a_NEON_HardFloat,
        CpuArchitecture_ARMEABI,
        CpuArchitecture_x86,
        CpuArchitecture_x86_64,
        CpuArchitecture_Mips,
        CpuArchitecture_Mips64,
        CpuArchitecture_ARM64v8a,
        CpuArchitecture_Unknown,
    };

    /**
     * エンディアンの種類を取得する
     */
    enum Endian_e {
        Endian_Little,
        Endian_Big,
    };

    /**
     * コンパイル時のエンディアン情報
     */
    const static Endian_e COMPILE_ENDIAN;

    struct VersionInfo {
        // メジャーバージョン
        uint32_t major = 0;

        // マイナーバージョン
        uint32_t minor = 0;

        // 表示用の名称
        string displayName;
    };

    virtual CpuArchitecture_e getCpuArchitecture() const;

    virtual const string getCpuArchitectureName() const;

    /**
     * 新たなThreadを生成させる
     */
    virtual void newThread(const std::string &name, const ThreadCallback callback) = 0;

    /**
     * アセット管理クラスを取得する
     */
    virtual std::shared_ptr<AssetManager> getAssetManager() const;

    /**
     * 文字列の変換クラスを取得する
     */
    virtual std::shared_ptr<IStringConverter> getStringConverter() const;

    /**
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform() const = 0;

    /**
     * 実行されているプラットフォームのバージョン情報を取得する。
     * 正常にバージョンを返せた場合はtrue, それ以外はfalseを返却する。
     */
    virtual bool getPlatformVersion(VersionInfo *result) const = 0;

    /**
     * 現在のプロセス情報を取得する
     */
    static std::shared_ptr<IProcessContext> getInstance();

    virtual ~IProcessContext() = default;

protected:
    IProcessContext();

    std::shared_ptr<AssetManager> assetManager;

    std::shared_ptr<IStringConverter> stringConverter;
};

}