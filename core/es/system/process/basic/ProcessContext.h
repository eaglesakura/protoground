#pragma once

#include "es/system/process/IProcessContext.h"

#if defined(BUILD_MacOSX)

namespace es {

class ProcessContext : public IProcessContext {
public:
    virtual ~ProcessContext();


    virtual void newThread(const std::string &name, const ThreadCallback callback) override;

    /**
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform() const override;

    /**
     * プロセス起動時に呼び出す
     */
    static void onBootProcess();

private:
    ProcessContext();
};

}

#endif
