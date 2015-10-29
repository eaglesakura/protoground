#pragma once

#include "es/protoground.hpp"
#include <es/system/Object.hpp>
#include "es/math/protoground-glm.hpp"
#include "es/graphics/image/IImageDecodeCallback.hpp"

namespace es {

class FontCharactor;
class Truetype;

/**
 * フォント1文字の外形情報を扱う
 */
class FontFace : public Object {
public:
    /**
     * ピクセル単位のフォントサイズを取得する。
     *
     * 文字はこのサイズに収まるようにレンダリングされる。
     */
    const i16vec2 &getSize();

    /**
     * フォントサイズを指定する
     *
     * レンダリングされる文字はこのwidth/heightに収まるように設定される。
     */
    virtual void setSize(const unsigned width, const unsigned height) = 0;

    /**
     * 指定のバッファに文字をレンダリングし、外径情報を返す。
     *
     * バッファがnullである場合はレンダリングを行わず、外形情報だけを返す。
     */
    virtual std::shared_ptr<FontCharactor> rendering(const wchar_t charactor, selection_ptr<IImageDecodeCallback> listener) = 0;


    /**
     * レンダリングをスレッドセーフに動作させる場合はtrue
     *
     * mutexを取得するので、多少動作が遅くなる。
     */
    void setThreadSafe(bool set);

    virtual ~FontFace() = default;

protected:

    FontFace() = default;

    /**
     * 現在のピクセル単位のフォントサイズ
     */
    i16vec2 size;

    bool threadSafe = false;
};

}


