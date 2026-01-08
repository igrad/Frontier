#include <AssetLoader.h>

#include <TestMacros.h>

#include <QSignalSpy>

#include <gtest/gtest.h>

using namespace Assets;
using namespace testing;

namespace
{
   constexpr const char* const TEST_IMAGE_FILE = ":/images/_TestImage.png";
   constexpr const char* const TEST_FONT_FILE = ":/fonts/_TestFont.ttf";
}

class AssetLoaderTest: public Test
{
public:
   AssetLoaderTest()
      : Loader()
      , FileNotFound(&Loader, &AssetLoaderInterface::FileNotFound)
      , FailedToLoadAsset(&Loader, &AssetLoaderInterface::FailedToLoadAsset)
      , ImageAssetLoaded(&Loader, &AssetLoaderInterface::ImageAssetLoaded)
      , FontAssetLoaded(&Loader, &AssetLoaderInterface::FontAssetLoaded)
   {

   }

   AssetLoader Loader;
   QSignalSpy FileNotFound;
   QSignalSpy FailedToLoadAsset;
   QSignalSpy ImageAssetLoaded;
   QSignalSpy FontAssetLoaded;
};

TEST_F(AssetLoaderTest, LoadImageAsset1)
{
   GWT("AssetLoader initialized",
       "LoadImageAsset is called on a nonexistant file",
       "FileNotFound signal is sent and AssetLoaded signal is not emit");


}
