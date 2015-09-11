#pragma once

#include <es/system/NanoTimer.h>
#include "es/protoground.hpp"
#include "es/framework/app/IApplication.hpp"

namespace es {

class BaseContext;

/**
 * 外部ライフサイクルから呼び出されるインターフェースを定義する
 *
 * 内部にContextを持ち、そちらのメソッドを呼び出す。
 * Contextを継承してアプリを構築することで、Thread呼び出し中のアプリ終了等で不正終了することを避ける。
 */
class ApplicationRunner : public virtual IApplication {
public:
    typedef std::function<void(std::shared_ptr<BaseContext>)> ThreadCallback;

    const std::shared_ptr<BaseContext> &getContext() const;

    virtual void onAppInitialize() override;

    virtual void onSurfaceChanged(int oldWidth, int oldHeight, int newWidth, int newHeight) override;

    virtual void onAppPause() override;

    virtual void onAppResume() override;

    virtual void onRequestRendering() override;

    virtual void onAppFrame() override;

    virtual void onAppFinalize() override;

    virtual void message(const Bundle &bundle);

    /**
     * Threadを開始する。
     * Contextの生存を強制するため、メインスレッドからしか生成することができない。
     */
    void startThread(const string &thraedName, const ThreadCallback &callback);

    /**
     * RunnerとContextを紐付けて管理する。
     */
    static void bind(std::shared_ptr<ApplicationRunner> runner, std::shared_ptr<BaseContext> context);


    /**
     * メッセージの識別ID
     */
    const static HashString MESSAGE_ID;

    enum MessageId_e {

        /**
         * ディスプレイにタッチされた
         */
                MessageId_TouchDisplay,
    };

    /**
     * 指やマウスを示す一意のID
     */
    const static HashString TOUCH_DISPLAY_UNIQUE_ID;

    /**
     * アクションの種類を示す
     */
    const static HashString TOUCH_DISPLAY_ACTION;

    /**
     * X座標:float
     */
    const static HashString TOUCH_DISPLAY_X;

    /**
     * Y座標:float
     */
    const static HashString TOUCH_DISPLAY_Y;

    enum TouchAction_e {
        TouchAction_Error = -1,
        TouchAction_Down = 0,
        TouchAction_Move,
        TouchAction_Up,
        TouchAction_Cancel,
    };
private:

    virtual void handleTouchDisplay(const Bundle &bundle);

    Hash64 mainThreadId;
    sp<BaseContext> context;

};


}


