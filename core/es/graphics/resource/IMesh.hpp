#pragma once


#include "es/protoground.hpp"
#include <es/graphics/engine/IGraphicsResource.h>
#include <es/graphics/engine/IDevice.hpp>

namespace es {

/**
 * 抽象化されたメッシュ
 */
class IMesh : public virtual IGraphicsResource {
public:
    struct LockOption;
    struct AllocOption;

    /**
     * メッシュをロックし、書き込みを可能にする。
     * ロックはtypeごとに一つだけ取得可能で、２つ以上のロックを確保しようとした場合このメソッドはnullを返却する。
     * ロック中にレンダリングを行った場合の動作は実装に依存する。
     */
    virtual void *lock(std::shared_ptr<IDevice> device, const LockOption &option) = 0;

    /**
     * メッシュのロックを解除し、書き込みを不可にする。
     * ロックされたポインタはunlock時点で無効になる。
     */
    virtual void unlock(std::shared_ptr<IDevice> device, const void *ptr) = 0;

    /**
     * メッシュメモリを確保する。
     *
     * 必ず必要量以上のメモリを確保するが、実際に確保される容量は実装に依存する。
     * データの0埋めは保証されない。
     */
    virtual void alloc(std::shared_ptr<IDevice> device, const AllocOption &option) = 0;

    /**
     * 頂点数を取得する
     */
    virtual uint getVertexCount() const = 0;

    /**
     * インデックス数を取得する
     */
    virtual uint getIndexCount() const = 0;

    enum Target_e {
        Target_Vertices,
        Target_Indices,
    };

    struct LockOption {
        /**
         * ロック対象のオフセット
         */
        uint offsetBytes = 0;

        /**
         * ロック対象のバイト数
         * 0の場合、全てのバッファをロックする
         */
        uint bytes = 0;

        /**
         * ロック対象
         */
        Target_e target = Target_Vertices;

        enum LockType_e {
            /**
             * 読み込みのみを行う
             */
                    LockType_ReadOnly,

            /**
             * 書き込みのみを行う
             */
                    LockType_WriteOnly,

            /**
             * 書き込みのみ、かつ古いデータの生存を保証する必要がない
             */
                    LockType_WriteOverwrite,


            /**
             * 読み書きの両方を行う
             */
                    LockType_ReadWrite,
        } type = LockType_WriteOnly;

        LockOption() = default;

        LockOption(const Target_e &target) : target(target) { };

        LockOption(const Target_e &target, const LockType_e &type) : target(target), type(type) { }
    };

    struct AllocOption {
        /**
         * データ数
         */
        uint num;

        /**
         * 1データのバイト数
         */
        uint onceBytes;

        /**
         * 確保対象
         */
        Target_e target = Target_Vertices;

        enum {
            /**
             * データは頻繁に書き換えられる
             */
                    Flag_Usage_Dynamic = 1 << 1,

            /**
             * データは一度しか利用されず、毎度書き換えられる
             */
                    Flag_Usage_Stream = 1 << 2,
        };
        uint flags = 0;


        AllocOption() = default;

        AllocOption(const Target_e &target) : target(target) { }
    };

    virtual ~IMesh() = default;
};

}