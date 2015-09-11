#pragma once

#include "es/protoground.hpp"
#include "mms/mms.hpp"
#include "es/graphics/model/file/ModelData.h"
#include "es/graphics/model/file/AnimationData.h"

namespace es {
namespace mms {

class MmsFileConverter {
public:
    /**
     * MMSモデルをシンプルなデータに変換する
     */
    static void convert(file::SkinMeshModelData *result, file::SymbolTable *resultSymbolTable, const std::shared_ptr<::mms::Model> model);

    /**
     * MMSモデルをシンプルなデータに変換する
     */
    static void convert(file::SkinMeshModelData *result, file::SymbolTable *resultSymbolTable, const std::shared_ptr<::mms::Model> model, const VertexAttribute *attributes);

    /**
     * MMSモーションをシンプルなデータに変換する
     */
    static void convert(file::AnimationData *result, file::SymbolTable *resultSymbolTable, const ::mms::MotionData *motion);
};


}
}


