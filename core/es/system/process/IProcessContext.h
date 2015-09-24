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
    };

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