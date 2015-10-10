﻿#pragma once

#include "es/protoground.hpp"
#include "es/math/Rect.hpp"

namespace es {

/**
 * 抽象化されたテクスチャ
 *
 */
class ITexture : public virtual Object {
public:
    typedef std::shared_ptr<ITexture> shared;

    enum Type_e {
        /**
         * 一次元テクスチャ
         */
                Type_1D,

        /**
         * 通常の二次元テクスチャ
         */
                Type_2D,

        /**
         * キューブマップ用テクスチャ
         */
                Type_CubeMap,

        /**
         * 外部リソース画像(SurfaceTexture等)
         * for Android
         */
                Type_AndroidExternalImage,

        /**
         * 不明なタイプ
         */
                Type_Unknown,
    };

    /**
     * Spriteの画像として利用可能なエリアを取得する
     *
     * @param existArea 有効なテクスチャのエリアを取得する
     */
    virtual void getImageArea(RectI16 *existArea) const = 0;

    /**
     * テクスチャ幅を取得する
     */
    virtual unsigned getWidth() const = 0;

    /**
     * テクスチャ高を取得する
     */
    virtual unsigned getHeight() const = 0;

    /**
     * テクスチャの種類を取得する
     */
    virtual Type_e getType() const = 0;


    virtual QueryResult_e queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const override {
        PGD_SUPPORT_QUERY(InterfaceId_Graphics_ITexture, ITexture);
        return Object::queryInterface(interfaceId, resultInterfacePtr);
    }

    virtual ~ITexture() = default;
};

}