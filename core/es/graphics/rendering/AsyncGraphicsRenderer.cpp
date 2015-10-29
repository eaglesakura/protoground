#include "AsyncGraphicsRenderer.h"

namespace es {

AsyncGraphicsRenderer::AsyncGraphicsRenderer() {

}

void AsyncGraphicsRenderer::addPacket(const AsyncGraphicsRenderer::packet &packet) {
    packetList.push_back(packet);
}

void AsyncGraphicsRenderer::addSaveObject(const std::shared_ptr<Object> obj) {
    saveObjects.push_back(obj);
}

void AsyncGraphicsRenderer::clear() {
    packetList.clear();
    saveObjects.clear();
}

}