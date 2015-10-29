#pragma once

#include "es/protoground.hpp"
#include "es/graphics/model/VertexAttribute.h"
#include <LinearMath/btIDebugDraw.h>
#include <es/graphics/Color.hpp>
#include <es/memory/SafeArray.hpp>

namespace es {

namespace debug {

/**
 * デバッグレンダリング用のデータを構築する
 *
 * Bulletのデバッグレンダラー互換となる。
 */
class GraphicsDebugDrawer : public btIDebugDraw {
public:
    GraphicsDebugDrawer();

    virtual void setDebugMode(int debugMode) override;

    virtual int getDebugMode() const override;

    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override;

    virtual void reportErrorWarning(const char *warningString) override;

    virtual void draw3dText(const btVector3 &location, const char *textString) override;

    virtual void flushLines() override;

    /**
     * 軸線を描画する
     */
    void drawRawAxis(const float length, const Color &xColor = Color::fromRGBAi(255, 0, 0, 255), const Color &yColor = Color::fromRGBAi(0, 255, 0, 255), const Color &zColor = Color::fromRGBAi(0, 0, 255, 255));

    /**
     * シンプルなラインを描画する
     */
    void drawLine(const vec3 &from, const vec3 &to, const Color &color);

    /**
     * 頂点のZ軸を反転する場合はtrueを指定する。
     *
     * 簡易的に右手・左手座標系を切り替える場合に利用する。
     */
    void setFripWorldZ(bool fripWorldZ);

    /**
     * 既存の頂点情報を削除する
     */
    void clear();

    struct Vertex {
        /**
         * 頂点位置
         */
        vec3 pos;

        /**
         * 頂点色
         */
        Color color;


        Vertex(const vec3 &pos, const Color &rgba) : pos(pos), color(rgba) { }

        Vertex(const btVector3 &pos, const btVector3 &rgb) {
            this->pos = vec3(pos.x(), pos.y(), pos.z());
            this->color = Color::fromRGBAf(rgb.x(), rgb.y(), rgb.z(), 1.0f);
        }

        Vertex() { }
    };

    unsafe_array<Vertex> getVertices() const;

    virtual ~GraphicsDebugDrawer() = default;

private:
    VertexAttribute::Complex vertexComplex;
    std::vector<Vertex> vertices;

    /**
     * 描画フラグ
     */
    int bulletDebugDrawFlag = DBG_DrawWireframe;

    /**
     * Z軸を反転する場合はtrue
     */
    bool fripWorldZ = false;
};


}


}

