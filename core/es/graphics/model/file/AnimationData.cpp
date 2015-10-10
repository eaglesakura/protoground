#include <es/asset/file/SerializeHeader.h>
#include <es/internal/log/Log.h>
#include "AnimationData.h"

namespace es {
namespace file {


bool AnimationData::someFrame(const AnimationData::BoneKeyFrame &a, const AnimationData::BoneKeyFrame &b) {
    return util::equals(a.pos, b.pos) &&
           util::equals(a.rotate, b.rotate);
}

ByteBuffer AnimationData::serialize(AnimationData::Serialize *animation) {
    assert(animation);

    animation->meta.boneKeyNum = animation->boneKeys.size();
    animation->meta.linkBoneNum = animation->linkBones.size();
    animation->meta.reserve0 = animation->meta.reserve1 = 0;

    const uint32_t boneKeyBytes = sizeof(AnimationData::BoneKeyFrame) * animation->meta.boneKeyNum;
    const uint32_t linkBoneBytes = sizeof(AnimationData::SymbolTimeline) * animation->meta.linkBoneNum;
    const uint32_t bufferSize = sizeof(Meta) + boneKeyBytes + linkBoneBytes;

    SerializeHeader header(SerializeHeader::DATA_UID_GRAPHICS_ANIMATION, bufferSize);

    ByteBuffer result = Buffer::create(bufferSize + sizeof(header));
    uint8_t *write = result.get();

    memcpy(write, &header, sizeof(header));
    write += sizeof(header);

    memcpy(write, &animation->meta, sizeof(Meta));
    write += sizeof(Meta);

    if (!animation->boneKeys.empty()) {
        memcpy(write, util::asPointer(animation->boneKeys), boneKeyBytes);
        write += boneKeyBytes;
    }

    if (!animation->linkBones.empty()) {
        memcpy(write, util::asPointer(animation->linkBones), linkBoneBytes);
        write += linkBoneBytes;
    }

    return result;
}

bool AnimationData::deserialize(AnimationData::Serialize *result, unsafe_array<uint8_t> buffer) {
    assert(result);

    SerializeHeader header;
    if (!SerializeHeader::deserialize(&buffer, &header, SerializeHeader::DATA_UID_GRAPHICS_ANIMATION)) {
        eslog("HeaderDeserialize Error");
        return false;
    }

    uint8_t *read = buffer.ptr;
    memcpy(&result->meta, read, sizeof(Meta));
    read += sizeof(Meta);

    const uint32_t boneKeyBytes = sizeof(BoneKeyFrame) * result->meta.boneKeyNum;
    const uint32_t linkBoneBytes = sizeof(SymbolTimeline) * result->meta.linkBoneNum;
    const uint32_t bufferSize = sizeof(Meta) + boneKeyBytes + linkBoneBytes;

    if (buffer.length < bufferSize) {
        return false;
    }


    if (boneKeyBytes) {
        util::valloc(&result->boneKeys, result->meta.boneKeyNum, false);
        memcpy(util::asPointer(result->boneKeys), read, boneKeyBytes);
        read += boneKeyBytes;
    }

    if (linkBoneBytes) {
        util::valloc(&result->linkBones, result->meta.linkBoneNum, false);
        memcpy(util::asPointer(result->linkBones), read, linkBoneBytes);
        read += linkBoneBytes;
    }

    return true;
}
}
}