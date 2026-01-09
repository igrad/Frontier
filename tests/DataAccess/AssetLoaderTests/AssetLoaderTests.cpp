#include <AssetLoader.h>

#include <TestMacros.h>

#include <QImage>
#include <QSignalSpy>

#include <gtest/gtest.h>

using namespace Assets;
using namespace testing;

namespace
{
   constexpr const char* const TEST_IMAGE_FILE = "://images/_TestImage.png";
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

   const QString fname(":/images/FileDoesNotExist.png");
   Loader.LoadImageAsset(fname);

   ASSERT_EQ(1, FileNotFound.count());
   EXPECT_EQ(1, FileNotFound.at(0).count());
   EXPECT_EQ(fname, FileNotFound.at(0).at(0).value<QString>());

   EXPECT_EQ(0, ImageAssetLoaded.count());
}

// TODO: How to make a file that passes "exists" but isn't readable?
TEST_F(AssetLoaderTest, DISABLED_LoadImageAsset2)
{
   GWT("AssetLoader initialized",
       "LoadImageAsset is called on a file that can't be read",
       "FailedToLoadAsset signal is sent and AssetLoaded signal is not emit");

   const QString fname(":/images/");
   Loader.LoadImageAsset(fname);

   EXPECT_EQ(0, FileNotFound.count());

   ASSERT_EQ(1, FailedToLoadAsset.count());
   EXPECT_EQ(1, FailedToLoadAsset.at(0).count());
   EXPECT_EQ(fname, FailedToLoadAsset.at(0).at(0).value<QString>());

   EXPECT_EQ(0, ImageAssetLoaded.count());
}

TEST_F(AssetLoaderTest, LoadImageAsset3)
{
   GWT("AssetLoader initialized",
       "LoadImageAsset is called on a real file that can be read",
       "AssetLoaded signal is emit");

   Loader.LoadImageAsset(TEST_IMAGE_FILE);

   // ASSERT_TRUE(ImageAssetLoaded.wait());
   ASSERT_EQ(1, ImageAssetLoaded.count());
   EXPECT_EQ(2, ImageAssetLoaded.at(0).count());
   EXPECT_EQ(TEST_IMAGE_FILE, ImageAssetLoaded.at(0).at(0).value<QString>());
   EXPECT_FALSE(ImageAssetLoaded.at(0).at(1).value<QImage>().isNull());
}
