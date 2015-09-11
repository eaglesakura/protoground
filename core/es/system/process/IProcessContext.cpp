#include "IProcessContext.h"
#include "es/asset/AssetManager.h"
#include "es/system/string/HashStringTable.h"

namespace es {

IProcessContext::IProcessContext() {
    HashStringTable::global();
    assetManager.reset(new AssetManager());

}

std::shared_ptr<AssetManager> IProcessContext::getAssetManager() const {
    return assetManager;
}

std::shared_ptr<IStringConverter> IProcessContext::getStringConverter() const {
    return stringConverter;
}
}