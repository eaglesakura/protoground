#pragma once

#include "estest/protoground-test.hpp"
#include "es/asset/IWriter.hpp"

namespace es {
namespace test {

/**
 * 適当なアセットを正常に読み込めることを検証する
 */
TEST(AssetLoaderTest, LoadTextFile) {
    ASSERT_TRUE((bool) IProcessContext::getInstance());
    ASSERT_TRUE((bool) IProcessContext::getInstance()->getAssetManager());

    std::shared_ptr<IAsset> asset = IProcessContext::getInstance()->getAssetManager()->load("gl/shader/SpriteShader.vsh");
    ASSERT_TRUE((bool) asset);
    ASSERT_TRUE(asset->available() > 0);
//    eslog("TestShader\n%s", util::toString(asset).c_str());
}

/**
 * 適当なテキストを正常に書き込めることを検証する
 */
TEST(AssetWriterTest, WriteTextFile) {
    ASSERT_TRUE((bool) IProcessContext::getInstance());
    ASSERT_TRUE((bool) IProcessContext::getInstance()->getAssetManager());

    auto writer = IProcessContext::getInstance()->getAssetManager()->write("test-text.txt");
    ASSERT_TRUE((bool) writer);

    ASSERT_TRUE(writer->write("ABC", 3));
    writer->close();
}

}
}