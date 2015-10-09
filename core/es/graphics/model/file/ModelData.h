#pragma once

#include "es/protoground.hpp"
#include "es/math/protoground-glm.hpp"
#include "es/graphics/Color.hpp"
#include "es/graphics/model/VertexAttribute.h"
#include "SymbolTable.h"
#include <es/memory/Buffer.hpp>

namespace es {

class IWriter;

class IAsset;

namespace file {

struct MeshData {
    PGD_FILE_ALIGN_OBJECT_BEGIN struct Meta {
        /**
         * 頂点属性情報
         */
        VertexAttribute::Complex vertexComplex;

        /**
         * 頂点数
         */
        uint32_t vertexNum = 0;

        /**
         * インデックス数
         */
        uint32_t indexNum = 0;

        /**
         * メッシュ全体のAABBを事前計算する。
         * これはカメラ位置のデフォルトを決めるとき等に利用できる。
         */
        struct {
            vec3 minPos;
            vec3 maxPos;
        } aabb;

        /**
         * インデックスの要素byte数
         * 1 | 2 | 4
         */
        uint8_t indexDataBytes = 0;
    }PGD_FILE_ALIGN_OBJECT_END;


    struct Serialize {
        Meta meta;
        std::shared_ptr<uint8_t> vertices;
        std::shared_ptr<uint8_t> indices;
    };

    /**
     * 計算可能なメタ情報を更新する
     */
    static void calcMeta(Serialize *serialize);

    static ByteBuffer serialize(Serialize *mesh);

    static bool deserialize(Serialize *result, const unsafe_array<uint8_t> buffer);
};

struct MaterialData {
    PGD_FILE_ALIGN_OBJECT_BEGIN struct Meta {
        uint8_t materialNum = 0;
        uint8_t textureNum = 0;

        uint8_t reserve0 = 0;
        uint8_t reserve1 = 0;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct Material {
        enum {
            Flag_NoCulling = 0x1 << 0,
            Flag_RenderingEdge = 0x1 << 1,
        };
        uint64_t symbol;

        /**
         * 上位ビットからRGBAの順番で指定される
         */
        PGD_FILE_ALIGN_OBJECT_BEGIN  struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } diffuse PGD_FILE_ALIGN_OBJECT_END;

        uint16_t flags;

        int16_t textureIndex;

        /**
         * レンダリングする頂点数
         */
        uint32_t indicesNum;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct Texture {
        uint64_t symbol;
    }PGD_FILE_ALIGN_OBJECT_END;

    struct Serialize {
        Meta meta;
        std::vector<Material> materials;
        std::vector<Texture> textures;
    };

    /**
     * テクスチャ番号を取得する
     * texturesに含まれていなければ末尾に追加する
     */
    static int8_t getTextureNumber(const string &texName, SymbolTable *table, std::vector<Texture> *textures);

    static ByteBuffer serialize(Serialize *material);

    static bool deserialize(Serialize *result, const unsafe_array<uint8_t> buffer);
};


struct BoneData {
    PGD_FILE_ALIGN_OBJECT_BEGIN  struct Meta {
        uint16_t boneNum = 0;
        uint16_t ikLinkNum = 0;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN  struct IkLink {
        enum {
            Flag_RotateLimited = 0x1 << 0,
        };
        /**
         *
         */
        uint16_t linkBoneIndex;
        uint16_t flags;
        vec3 minRadian;
        vec3 maxRadian;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN   struct Bone {
        /**
         * 初期位置
         */
        vec3 pos;

        /**
         * 初期回転角
         */
        quat rotate;
        /**
         * ボーン名
         */
        uint64_t symbol;

        /**
         * 親ボーン番号
         *
         * 親を持たない場合は負の値となる。
         */
        int16_t parentBoneIndex;

        /**
         * IK接続対象
         *
         * IK接続対象が存在しない場合は負の値となる。
         */
        int16_t ikTargetBoneIndex;

        /**
         * 利用するIKリンクのデータ位置
         *
         * IK接続対象が存在しない場合は常に0
         * ikLinkTable[ikLinkOffset] - ikLinkTable[ikLinkNum - 1] までを利用する。
         */
        uint16_t ikLinkOffset;

        /**
         * IKの最大制御角度をラジアンで示す
         */
        float ikLimitedRadian;

        /**
         * 所有しているIKリンク数
         * 0の場合はIKを保持・計算しない
         */
        uint16_t ikLinkNum;

        /**
         * IK計算ループ数
         */
        uint8_t ikLoopCount;

    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct Serialize {
        Meta meta;
        std::vector<IkLink> ikLinks;
        std::vector<Bone> bones;
    };

    static ByteBuffer serialize(Serialize *bone);

    static bool deserialize(Serialize *result, const unsafe_array<uint8_t> buffer);
};

struct MorphData {
    enum Type_e {
        Type_Position = 0,
        Type_UV = 0,
    };

    PGD_FILE_ALIGN_OBJECT_BEGIN struct Meta {
        uint16_t positionMorphNum = 0;

        uint16_t uvMorphNum = 0;

        uint16_t targetNum = 0;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct Target {
        uint64_t symbol;

        /**
         * データタイプ
         */
        uint16_t type;

        /**
         * データの開始位置
         */
        uint32_t dataOffset;

        /**
         * データ個数
         */
        uint32_t dataNum;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct PositionMorph {
        /**
         * 移動量
         */
        vec3 offset;

        /**
         * 頂点インデックス
         */
        int32_t index;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct UvMorph {
        /**
         * 移動量
         */
        vec2 offset;

        /**
         * 頂点インデックス
         */
        uint32_t index;
    }PGD_FILE_ALIGN_OBJECT_END;

    struct Serialize {
        Meta meta;
        std::vector<Target> targets;
        std::vector<PositionMorph> positionMorphs;
        std::vector<UvMorph> uvMorphs;
    };

    static ByteBuffer serialize(Serialize *morph);

    static bool deserialize(Serialize *result, const unsafe_array<uint8_t> buffer);
};

/**
 * Bullet Physicsをベースとした物理情報
 */
struct PhysicsData {
    enum ShapeType_e {
        ShapeType_Sphere,
        ShapeType_Box,
        ShapeType_Capsule,
    };

    enum {
        PhysicsFlag_BoneLink = 0x1 << 0,
        PhysicsFlag_Physics = 0x1 << 1,
    };

    PGD_FILE_ALIGN_OBJECT_BEGIN struct Meta {
        uint16_t bodyNum;
        uint16_t jointNum;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct RigidBody {
        uint64_t symbol;

        int16_t linkBoneIndex;

        /**
         * 非判定グループフラグ
         */
        uint16_t noCollisionGroups;

        /**
         * 所属グループフラグ
         */
        uint16_t groupFlags;

        /**
         * 物理制御フラグ
         */
        uint8_t physicsFlags;

        /**
         * 物理形状
         */
        uint8_t shapeType;

        vec3 position;
        vec3 rotate;
        vec3 size;
        float mass;
        float translateAttenuation;
        float rotateAttenuation;
        float elasticity;
        float friction;
    }PGD_FILE_ALIGN_OBJECT_END;

    PGD_FILE_ALIGN_OBJECT_BEGIN struct RigidJoint {
        uint64_t symbol;

        int16_t rigidBodyIndex;

        int16_t connectRigidBodyIndex;

        /**
         * 位置
         */
        vec3 position;

        /**
         * 回転オイラー角
         */
        vec3 rotate;

        /**
         * 移動の最小値
         */
        vec3 translateMinLimit;

        /**
         * 移動の最大値
         */
        vec3 translateMaxLimit;

        vec3 rotateMinLimit;

        vec3 rotateMaxLimit;

        /**
         * バネの移動値
         */
        vec3 springTranslate;

        /**
         * バネの回転値
         */
        vec3 springRotate;
    }PGD_FILE_ALIGN_OBJECT_END;

    struct Serialize {
        Meta meta;
        std::vector<RigidBody> rigidBodies;
        std::vector<RigidJoint> rigidJoints;
    };

    static ByteBuffer serialize(Serialize *physics);

    static bool deserialize(Serialize *result, const unsafe_array<uint8_t> buffer);
};

/**
 * MMS/FBXサブセットに相当するデータを構築する
 */
struct SkinMeshModelData {
    MeshData::Serialize mesh;
    MaterialData::Serialize material;
    BoneData::Serialize bone;
    MorphData::Serialize morph;
    PhysicsData::Serialize physics;

    static ByteBuffer serialize(SkinMeshModelData *model);

    static bool deserialize(SkinMeshModelData *result, unsafe_array<uint8_t> buffer);

private:
    PGD_FILE_ALIGN_OBJECT_BEGIN struct Meta {
        uint32_t meshDataBytes;
        uint32_t materialDataBytes;
        uint32_t boneDataBytes;
        uint32_t morphDataBytes;
        uint32_t physicsDataBytes;
    }PGD_FILE_ALIGN_OBJECT_END;
};

}

}



