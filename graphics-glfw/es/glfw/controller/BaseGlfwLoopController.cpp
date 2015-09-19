#include <es/system/Bundle.h>
#include <es/framework/app/ApplicationRunner.h>
#include "es/system/NanoTimer.h"
#include "BaseGlfwLoopController.h"
#include "es/system/Thread.hpp"
#include "es/internal/protoground-internal.hpp"
#include "es/gl-compat.h"


namespace es {

BaseGlfwLoopController::BaseGlfwLoopController(std::shared_ptr<GlfwDevice> windowDevice) :
        device(windowDevice) {
    glfwSetWindowUserPointer(windowDevice->getWindow(), this);
    glfwSetCursorPosCallback(windowDevice->getWindow(), BaseGlfwLoopController::cursorCallback);
    glfwSetMouseButtonCallback(windowDevice->getWindow(), BaseGlfwLoopController::mouseCallback);
}

int BaseGlfwLoopController::execute() {
    device->bind();

    // 初期化
    onLoopInitialize();

    i16vec2 oldSurfaceSize;
    bool oldWindowVisible = false;

    const int SLEEP_TIME = 1000 / 120;   // 何らかの原因でsleepさせる場合の休止時間

    while (true) {
        device->pollEvents();
        if (device->isWindowClose()) {
            break;
        }

        // 可視状態チェック
        bool nowWindowVisible = device->isWindowVisible();
        if (nowWindowVisible != oldWindowVisible) {
            // 可視状態が変更された
            oldWindowVisible = nowWindowVisible;

            if (nowWindowVisible) {
                eslog("onResume(%x)", this);
                loopState = LoopState::Run;
                device->bind();
                onLoopResume();
            } else {
                eslog("onPause(%x)", this);
                loopState = LoopState::Pause;
                onLoopPaused();
                device->unbind();
            }
        }

        if (loopState == LoopState::Run) {
            // サーフェイスサイズチェック
            auto nowSurfaceSize = device->getFramebufferSize();

            if (nowSurfaceSize != oldSurfaceSize) {
                eslog("onSurfaceChanged(%x) old(%dx%d) -> new(%dx%d)", this,
                      (int) oldSurfaceSize.x, (int) oldSurfaceSize.y,
                      (int) nowSurfaceSize.x, (int) nowSurfaceSize.y
                );
                onLoopSurfaceSizeChanged(oldSurfaceSize.x, oldSurfaceSize.y, nowSurfaceSize.x, nowSurfaceSize.y);
                oldSurfaceSize = nowSurfaceSize;
            }

            onLoopFrame();
        } else {
            NanoTimer::sleepMs(SLEEP_TIME);
        }
    }

    // 解放処理を行う
    {
        device->bind();
        eslog("onLoopFinish(%x)", this);
        onLoopFinish();
        device->unbind();
    }

    return 0;
}

void BaseGlfwLoopController::onMouseAction(int button, int action, float x, float y) {
    Bundle bundle;
    bundle.putInt32(ApplicationRunner::MESSAGE_ID, ApplicationRunner::MessageId_TouchDisplay);
    bundle.putInt32(ApplicationRunner::TOUCH_DISPLAY_UNIQUE_ID, button);
    bundle.putFloat(ApplicationRunner::TOUCH_DISPLAY_X, x);
    bundle.putFloat(ApplicationRunner::TOUCH_DISPLAY_Y, y);
    if (action == GLFW_PRESS) {
        bundle.putInt32(ApplicationRunner::TOUCH_DISPLAY_ACTION, ApplicationRunner::TouchAction_Down);
    } else if (action == GLFW_RELEASE) {
        bundle.putInt32(ApplicationRunner::TOUCH_DISPLAY_ACTION, ApplicationRunner::TouchAction_Up);
    } else if (action == ACTION_MOVE) {
        bundle.putInt32(ApplicationRunner::TOUCH_DISPLAY_ACTION, ApplicationRunner::TouchAction_Move);
    }

    if (action != ACTION_MOVE) {
        eslog("Mouse Action(%d) button(%d) pos(%.1f, %.1f)", action, button, (float) x, (float) y);
    }
    onMouseAction(bundle);
}

void BaseGlfwLoopController::cursorCallback(GLFWwindow *window, double x, double y) {
    BaseGlfwLoopController *self = (BaseGlfwLoopController *) glfwGetWindowUserPointer(window);
//    eslog("BaseGlfwLoopController::cursorCallback pos(%.2f, %.2f)", (float) x, (float) y);
    const float scale = self->device->getWindowScale();
    self->mousePos.x = (float) x * scale;
    self->mousePos.y = (float) y * scale;

    int index = 0;
    for (const auto &button : self->buttonContext) {
        if (button.action == GLFW_PRESS) {
            // 押されているキーがあればMoveとして認識
            self->onMouseAction(index, ACTION_MOVE, self->mousePos.x, self->mousePos.y);
        }
        ++index;
    }
}

void BaseGlfwLoopController::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button < 0 || button > MAX_MOUSE_BUTTON) {
        return;
    }


    BaseGlfwLoopController *self = (BaseGlfwLoopController *) glfwGetWindowUserPointer(window);

    // アクションを保存
    self->buttonContext[button].action = action;
    self->onMouseAction(button, action, self->mousePos.x, self->mousePos.y);
}

}