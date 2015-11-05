#include "GraphicsDebugDrawer.h"
#include "es/graphics/model/pose/BoneController.h"

namespace es {
namespace debug {


GraphicsDebugDrawer::GraphicsDebugDrawer() {
    {
        VertexAttribute attr[] = {
                VertexAttribute::POSITION_float3,
                VertexAttribute::COLOR_i4,
                VertexAttribute::END
        };

        vertexComplex = VertexAttribute::makeComplex(attr);
    }
}

void GraphicsDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    if (fripWorldZ) {
        vertices.push_back(Vertex(btVector3(from.x(), from.y(), -from.z()), color));
        vertices.push_back(Vertex(btVector3(to.x(), to.y(), -to.z()), color));
    } else {
        vertices.push_back(Vertex(from, color));
        vertices.push_back(Vertex(to, color));
    }
}

void GraphicsDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) {

}

void GraphicsDebugDrawer::reportErrorWarning(const char *warningString) {

}

void GraphicsDebugDrawer::draw3dText(const btVector3 &location, const char *textString) {

}

void GraphicsDebugDrawer::flushLines() {
    btIDebugDraw::flushLines();
}

void GraphicsDebugDrawer::drawRawAxis(const float length, const Color &xColor, const Color &yColor, const Color &zColor) {
    vertices.push_back(Vertex(vec3(0, 0, 0), xColor));
    vertices.push_back(Vertex(vec3(length, 0, 0), xColor));

    vertices.push_back(Vertex(vec3(0, 0, 0), yColor));
    vertices.push_back(Vertex(vec3(0, length, 0), yColor));

    vertices.push_back(Vertex(vec3(0, 0, 0), zColor));
    vertices.push_back(Vertex(vec3(0, 0, length), zColor));
}

void GraphicsDebugDrawer::drawLine(const vec3 &from, const vec3 &to, const Color &color) {
    if (fripWorldZ) {
        vertices.push_back(Vertex(vec3(from.x, from.y, -from.z), color));
        vertices.push_back(Vertex(vec3(to.x, to.y, -to.z), color));
    } else {
        vertices.push_back(Vertex(from, color));
        vertices.push_back(Vertex(to, color));
    }
}

void GraphicsDebugDrawer::setFripWorldZ(bool fripWorldZ) {
    this->fripWorldZ = fripWorldZ;
}

void GraphicsDebugDrawer::clear() {
    vertices.clear();
}

void GraphicsDebugDrawer::setDebugMode(int debugMode) {
    bulletDebugDrawFlag = debugMode;
}

int GraphicsDebugDrawer::getDebugMode() const {
    return this->bulletDebugDrawFlag;
}

unsafe_array<GraphicsDebugDrawer::Vertex> GraphicsDebugDrawer::getVertices() const {
    return util::unsafe(vertices);
}

}
}