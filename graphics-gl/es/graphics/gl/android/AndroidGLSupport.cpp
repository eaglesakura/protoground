#ifdef __ANDROID__

#include <EGL/egl.h>
#include <es/internal/log/Log.h>

namespace es {
namespace gl {

void *pglEglGetGlProcAddress(void *, char *name) {
    void *addr = (void *) eglGetProcAddress(name);
    if (!addr) {
        eslog("GL Proc fail(%s)", name);
    } else {
        eslog("%s = %x", name, addr);
    }
    return addr;
}

}
}

#endif /* __ANDROID__ */
