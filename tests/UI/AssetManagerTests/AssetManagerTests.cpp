#include <AssetManager.h>

#include <AssetLoader/AssetLoaderMock.h>

#include <TestMacros.h>

#include <QFont>
#include <QFontDatabase>
#include <QImage>
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Assets;
using namespace testing;

namespace
{
   constexpr const char* const TEST_FONT_PATH = "://fonts/_TestFont.ttf";
}

class AssetManagerTest: public Test
{
public:
   AssetManagerTest()
      : Loader()
      , Manager(XPtr<AssetLoaderInterface>(&Loader))
   {

   }

   NiceMock<AssetLoaderMock> Loader;
   AssetManager Manager;
};

TEST_F(AssetManagerTest, FontLoaded1)
{
   QSignalSpy spy(&Manager, &AssetManager::FontLoaded);

   const QString path = "path";
   const QFont font;
   Loader.EmitFontAssetLoaded(path, font);

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy[0].count());
   EXPECT_EQ(path, spy[0][0]);
   EXPECT_EQ(font, spy[0][1].value<QFont>());
}

TEST_F(AssetManagerTest, ImageLoaded1)
{
   QSignalSpy spy(&Manager, &AssetManager::ImageLoaded);

   const QString path = "path";
   const QImage image;
   Loader.EmitImageAssetLoaded(path, image);

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy[0].count());
   EXPECT_EQ(path, spy[0][0]);
   EXPECT_EQ(image, spy[0][1].value<QImage>());
}

TEST_F(AssetManagerTest, IsAssetAvailable1)
{
   const QString fontPath = "fontPath";
   const QFont font;
   EXPECT_FALSE(Manager.IsAssetAvailable(fontPath));
   Loader.EmitFontAssetLoaded(fontPath, font);
   EXPECT_TRUE(Manager.IsAssetAvailable(fontPath));

   const QString imagePath = "imagePath";
   const QImage image;
   EXPECT_FALSE(Manager.IsAssetAvailable(imagePath));
   Loader.EmitImageAssetLoaded(imagePath, image);
   EXPECT_TRUE(Manager.IsAssetAvailable(imagePath));
}

TEST_F(AssetManagerTest, GetFont1)
{
   const QString fontPath = TEST_FONT_PATH;

   EXPECT_EQ(QFont(), Manager.GetFont(fontPath));
   const QFont font(fontPath);
   Loader.EmitFontAssetLoaded(fontPath, font);
   EXPECT_EQ(font, Manager.GetFont(fontPath));
}
