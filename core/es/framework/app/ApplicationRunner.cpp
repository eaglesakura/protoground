#include "ApplicationRunner.h"
#include <es/system/process/IProcessContext.h>
#include <es/system/string/HashStringTable.h>
#include "es/framework/app/BaseContext.h"
#include "es/internal/protoground-internal.hpp"
#include "es/framework/touch/TouchPoint.h"

namespace es {

void ApplicationRunner::onAppInitialize() {
    assert(context);

    mainThreadId = Hash64::fromCurrentThread();
    context->onAppInitialize();
}

void ApplicationRunner::onSurfaceChanged(int oldWidth, int oldHeight, int newWidth, int newHeight) {
    context->onSurfaceChanged(oldWidth, oldHeight, newWidth, newHeight);
}

void ApplicationRunner::onAppPause() {
    context->onAppPause();
}

void ApplicationRunner::onAppResume() {
    context->onAppResume();
}

void ApplicationRunner::onRequestRendering() {
    context->onRequestRendering();
}

void ApplicationRunner::onAppFrame() {
    context->onAppFrame();
}

void ApplicationRunner::onAppFinalize() {
    context->onAppFinalize();
    context.reset();
}


void ApplicationRunner::startThread(const string &thraedName, const ApplicationRunner::ThreadCallback &callback) {
    assert(Hash64::fromCurrentThread() == mainThreadId);
    auto saveContext = this->context;
    IProcessContext::getInstance()->newThread(thraedName, [callback, saveContext]() {
        callback(saveContext);
    });
}

const std::shared_ptr<BaseContext> &ApplicationRunner::getContext() const {
    return context;
}


void ApplicationRunner::bind(std::shared_ptr<ApplicationRunner> runner, std::shared_ptr<BaseContext> context) {
    assert(runner);
    assert(context);
    assert(!runner->context);
    assert(!context->runnerRef.lock());
    assert(!context->selfRef.lock());

    // 相互参照を組み立てる
    runner->context = context;
    context->runnerRef = runner;
    context->selfRef = context;
}

/**
 * メッセージ識別ID
 */
ES_GLOBAL_HASH_IMPL(ApplicationRunner, MESSAGE_ID);


/**
 * 指やマウスを示す一意のID
 */
ES_GLOBAL_HASH_IMPL(ApplicationRunner, TOUCH_DISPLAY_UNIQUE_ID);

/**
 * アクションの種類を示す
 */
ES_GLOBAL_HASH_IMPL(ApplicationRunner, TOUCH_DISPLAY_ACTION);

/**
 * X座標:float
 */
ES_GLOBAL_HASH_IMPL(ApplicationRunner, TOUCH_DISPLAY_X);

/**
 * Y座標:float
 */
ES_GLOBAL_HASH_IMPL(ApplicationRunner, TOUCH_DISPLAY_Y);

void ApplicationRunner::message(const Bundle &bundle) {
    switch (bundle.getInt32(MESSAGE_ID, -1)) {
        case MessageId_TouchDisplay: {
            handleTouchDisplay(bundle);
        }
            break;
    }
}

void ApplicationRunner::handleTouchDisplay(const Bundle &bundle) {
    float touchX = bundle.getFloat(TOUCH_DISPLAY_X, -1);
    float touchY = bundle.getFloat(TOUCH_DISPLAY_Y, -1);
    int action = bundle.getInt32(TOUCH_DISPLAY_ACTION, TouchAction_Error);
    int touchId = bundle.getInt32(TOUCH_DISPLAY_UNIQUE_ID, 0xFFEEFFEE);

    if (touchX < 0 || touchY < 0 || action == TouchAction_Error || touchId == 0xFFEEFFEE) {
        return;
    }

    TouchEvent event;
    event.position.x = touchX;
    event.position.y = touchY;
    event.action = (TouchEvent::Action_e) action;
    event.id = touchId;
    auto context = this->context;
    context->postTask(mainThreadId, [context, event](TaskQueue *, const Hash64 &id) {
        context->onDisplayTouch(event);
    });
}
}