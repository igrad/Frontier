#include <AssetManager.h>

#include <AssetLoader/AssetLoaderMock.h>

#include <TestMacros.h>
#include <Log.h>

#include <QFont>
#include <QFontDatabase>
#include <QImage>
#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Assets;
using namespace testing;

namespace
{
   constexpr const char* const TEST_IMAGE_PATH = "://images/_TestImage.png";
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

TEST_F(AssetManagerTest, RequestFont1)
{
   const QString fontPath = TEST_FONT_PATH;
   EXPECT_CALL(Loader, LoadFontAsset(fontPath));
   Manager.RequestFont(fontPath);
}

TEST_F(AssetManagerTest, RequestFont2)
{
   const QString fontPath = TEST_FONT_PATH;

   Manager.RequestFont(fontPath);

   EXPECT_CALL(Loader, LoadFontAsset(fontPath)).Times(0);
   Manager.RequestFont(fontPath);
}

TEST_F(AssetManagerTest, RequestFont3)
{
   const QString fontPath = TEST_FONT_PATH;
   const QFont font(fontPath);

   Loader.EmitFontAssetLoaded(fontPath, font);

   EXPECT_CALL(Loader, LoadFontAsset(fontPath)).Times(0);
   Manager.RequestFont(fontPath);
}

TEST_F(AssetManagerTest, RequestImage1)
{
   const QString imagePath = TEST_IMAGE_PATH;

   EXPECT_CALL(Loader, LoadImageAsset(imagePath));
   Manager.RequestImage(imagePath);
}

TEST_F(AssetManagerTest, RequestImage2)
{
   const QString imagePath = TEST_IMAGE_PATH;

   Manager.RequestImage(imagePath);

   EXPECT_CALL(Loader, LoadImageAsset(imagePath)).Times(0);
   Manager.RequestImage(imagePath);
}

TEST_F(AssetManagerTest, RequestImage3)
{
   const QString imagePath = TEST_IMAGE_PATH;

   Loader.EmitImageAssetLoaded(imagePath, QImage());

   EXPECT_CALL(Loader, LoadImageAsset(imagePath)).Times(0);
   Manager.RequestImage(imagePath);
}
