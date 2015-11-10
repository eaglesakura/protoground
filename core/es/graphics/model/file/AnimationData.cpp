#include <es/asset/file/SerializeHeader.h>
#include <es/internal/log/Log.h>
#include "AnimationData.h"
#include "es/asset/file/JsonData.hpp"
#include "es/graphics/model/file/SymbolTable.h"

namespace es {
namespace file {


bool AnimationData::someFrame(const AnimationData::BoneKeyFrame &a, const AnimationData::BoneKeyFrame &b) {
    return util::equals(a.pos, b.pos) &&
        util::equals(a.rotate, b.rotate);
}

void AnimationData::parseAnimationSliceJson(const string &json, SymbolTable *symbols, std::vector<Slice>* result) {
    file::SymbolTable tempTable;
    if (!symbols) {
        symbols = &tempTable;
    }

    sp<Json> data = Json::parse(json);

    auto slice = data->getChild("slice");
    assert(slice->getType() == Json::JsonType_Array);

    for (unsigned i = 0; i < slice->getChildCount(); ++i) {
        auto item = slice->getChild(i);

        AnimationData::Slice slice = { 0 };

        string name;

        if (item->get("name", &name) &&
            item->getAs("startFrame", &slice.startFrame) &&
            item->getAs("numFrame", &slice.numFrame)) {

            slice.symbol = symbols->add(name);

            eslog(" Animation Slice[%d] name(%s) startFrame(%d) numFrame(%d)", i, name.c_str(), slice.startFrame, slice.numFrame);

            result->push_back(slice);
        }
    }

}

ByteBuffer AnimationData::serialize(AnimationData::Serialize *animation) {
    assert(animation);

    animation->meta.boneKeyNum = animation->boneKeys.size();
    animation->meta.linkBoneNum = animation->linkBones.size();
    animation->meta.sliceNum = animation->slices.size();

    const uint32_t boneKeyBytes = sizeof(AnimationData::BoneKeyFrame) * animation->meta.boneKeyNum;
    const uint32_t linkBoneBytes = sizeof(AnimationData::SymbolTimeline) * animation->meta.linkBoneNum;
    const uint32_t sliceBytes = sizeof(AnimationData::Slice) * animation->meta.sliceNum;
    const uint32_t bufferSize = sizeof(Meta) + boneKeyBytes + linkBoneBytes + sliceBytes;

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
    if (!animation->slices.empty()) {
        memcpy(write, util::asPointer(animation->slices), sliceBytes);
        write += sliceBytes;
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
    const uint32_t sliceBytes = sizeof(Slice) * result->meta.sliceNum;
    const uint32_t bufferSize = sizeof(Meta) + boneKeyBytes + linkBoneBytes;

    if (buffer.length < (int) bufferSize) {
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

    if (sliceBytes) {
        util::valloc(&result->slices, result->meta.sliceNum, false);
        memcpy(util::asPointer(result->slices), read, sliceBytes);
        read += sliceBytes;
    }

    return true;
}
}
}