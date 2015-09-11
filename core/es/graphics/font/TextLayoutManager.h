#pragma once

#include "es/protoground.hpp"
#include "es/graphics/font/FontCharactor.h"
#include "es/graphics/font/FontFace.h"

#include <vector>
#include <es/math/Rect.hpp>


namespace es {

class TextLayoutManager : public Object {
public:
    class TextItem;

    struct LayoutOption;

    TextLayoutManager(const std::shared_ptr<FontFace> &face);

    virtual ~TextLayoutManager() = default;

    /**
     * 文字を追加する
     * 追加に成功したらTextItemを返却する。
     * 描画領域をはみ出した等の理由で追加できなかったらnullptrを返却して何もしない。
     */
    std::shared_ptr<TextItem> add(const std::shared_ptr<FontCharactor> &charactor, const LayoutOption *option = nullptr);

    /**
     * 指定した間隔を開ける
     */
    void addSpace(const uint xPixels);

    /**
     * 改行を行う
     */
    bool newLine(const LayoutOption *option = nullptr);

    /**
     * MaxLinesの最後にFooderとして指定される文字列
     * "..."等を加える。
     */
    void setFooderText(const std::vector<std::shared_ptr<FontCharactor> > charactors);

    /**
     * レイアウトに配置されるサイズを指定する
     */
    void setLayoutSize(const uint width, const uint height);

    /**
     * レイアウトに配置されるサイズを取得する
     */
    const i16vec2 &getLayoutSize() const;

    /**
     * レイアウトされる最大行数を指定する
     */
    void setMaxLines(const uint lines);

    /**
     * 最大行数を取得する
     */
    uint getMaxLines() const;

    /**
     * 最終行をレイアウト中である場合trueを返す
     */
    bool isLastLine() const;

    /**
     * テキストリストを取得する
     */
    const std::vector<std::shared_ptr<TextItem> > &getText() const;

    class TextItem {
    public:
        const std::shared_ptr<FontCharactor> &getCharactor() const;

        /**
         * フォントの配置を取得する
         */
        const RectI16 &getFontArea() const;

        /**
         * Bitmapの配置を取得する
         */
        const RectI16 &getBitmapArea() const;

        /**
         * 配置されている行を取得する
         */
        const uint getLine() const;

    private:
        /**
         * 配置される文字
         */
        sp<FontCharactor> charactor;

        /**
         * 配置されている行
         */
        int16_t line;

        /**
         * フォントが配置されている位置
         */
        RectI16 fontArea;

        /**
         * 描画エリア
         *
         * bitmapが描画される位置を保存する
         */
        RectI16 bitmapArea;

        friend class TextLayoutManager;
    };

    struct LayoutOption {
    };

private:
    /**
     * 配置するサイズ
     */
    i16vec2 size;

    /**
     * 次に配置する位置
     * レイアウトはベースラインを基礎とするので、ベースライン位置を指定する
     */
    i16vec2 nextBaselinePosition;

    /**
     * 改行時のYオフセット量を取得する
     */
    int16_t offsetLineY = 0;

    /**
     * 配置するベースライン
     */
    int16_t baselinePositionY = 0;

    /**
     * 最大行数
     * 0の場合は制限を加えない
     * 指定されている場合、sizeを超えるからmaxLinesを超えるまで配置される
     */
    int16_t maxLines = 0;

    /**
     * 現在描画しているライン数
     */
    int16_t currentLine = 0;

    /**
     * 描画対象のテキストリスト
     */
    std::vector<std::shared_ptr<TextItem> > text;

    /**
     * フッダテキスト
     */
    std::vector<std::shared_ptr<FontCharactor> > foodefs;

    /**
     * フッダ領域を事前計算する
     */
    int16_t fooderWidth = 0;
};

}