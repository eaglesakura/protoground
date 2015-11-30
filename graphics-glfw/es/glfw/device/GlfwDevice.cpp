#include "GlfwDevice.h"
#include "es/internal/protoground-internal.hpp"
#include <GLFW/glfw3.h>

#if defined(BUILD_Cygwin) && defined(PGD_GLFW_DUMMY_FUNCTIONS)
// Cygwin用にカスタムビルドしたGLFW用のダミー関数
extern "C" {
int __ms_vsnprintf(char *, const char *, ...) {
    return 0;
}

void _wassert(bool) {

}
}
#endif

namespace es {

std::atomic<int> GlfwDevice::existDevices;

GlfwDevice::GlfwDevice(GLFWwindow *newWindow) : window(newWindow) {
    ++existDevices;
}

GlfwDevice::~GlfwDevice() {

    if (window) {
        if (glfwGetCurrentContext() == window) {
            glfwMakeContextCurrent(nullptr);
        }
        glfwDestroyWindow(window);
    }

    --existDevices;
    assert(existDevices >= 0);

    // 全てのデバイスが無くなったら解放する
    if (existDevices == 0) {
        glfwTerminate();
    }
}

bool GlfwDevice::isWindowClose() const {
    return glfwWindowShouldClose(window) ? true : false;
}

void GlfwDevice::swapBuffers() {
    glfwSwapBuffers(window);
}

std::shared_ptr<GlfwDevice> GlfwDevice::createInstance(const unsigned width, const unsigned height, const bool resizeable, const std::string &title, const std::shared_ptr<GlfwDevice> sharedContext) {

    glfwSetErrorCallback([](int error, const char *msg) {
        eslog("GLFW Error(%x) Message(%s)", error, msg);
    });

    std::shared_ptr<GlfwDevice> result;
    if (!sharedContext && !existDevices) {
        if (!glfwInit()) {
            return result;
        }
    }


    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, resizeable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FOCUSED, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), nullptr, sharedContext ? sharedContext->window : nullptr);
    if (!window) {
        return result;
    }

    result.reset(new GlfwDevice(window));

    glfwMakeContextCurrent(nullptr);
    return result;
}

std::shared_ptr<GlfwDevice> GlfwDevice::createOffscreenInstance(const unsigned width, const unsigned height, const std::shared_ptr<GlfwDevice> sharedContext) {
    std::shared_ptr<GlfwDevice> result;
    if (!sharedContext && !existDevices) {
        if (!glfwInit()) {
            return result;
        }
    }


    glfwSetErrorCallback([](int error, const char *msg) {
        eslog("GLFW Error(%x) Message(%s)", error, msg);
    });

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(width, height, "", nullptr, sharedContext ? sharedContext->window : nullptr);
    if (!window) {
        return result;
    }

    result.reset(new GlfwDevice(window));
    glfwMakeContextCurrent(nullptr);
    return result;
}

void GlfwDevice::requestCloseWindow() {
    glfwSetWindowShouldClose(window, GL_FALSE);
}

void GlfwDevice::pollEvents() {
    glfwPollEvents();
}

void GlfwDevice::bind() {
    glfwMakeContextCurrent(window);
}

void GlfwDevice::unbind() {
    // GPUコマンドキューの処理完了を行わなければならない
    glFinish();
    glfwMakeContextCurrent(nullptr);
}

i16vec2 GlfwDevice::getWindowSize() const {
    i32vec2 result;
    glfwGetWindowSize(window, &result.x, &result.y);
    return i16vec2((int16_t) result.x, (int16_t) result.y);
}

bool GlfwDevice::isWindowVisible() const {
    return glfwGetWindowAttrib(window, GLFW_VISIBLE) ? true : false;
}

i16vec2 GlfwDevice::getFramebufferSize() const {
    i32vec2 result;
    glfwGetFramebufferSize(window, &result.x, &result.y);
    return i16vec2((int16_t) result.x, (int16_t) result.y);
}

float GlfwDevice::getWindowScale() const {
    return (float) getFramebufferSize().x / (float) getWindowSize().x;
}

GLFWwindow *GlfwDevice::getWindow() const {
    return window;
}
}