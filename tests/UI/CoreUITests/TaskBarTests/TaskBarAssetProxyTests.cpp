#include <TaskBar/TaskBarAssetProxy.h>

#include <AssetManager/AssetClientMock.h>
#include <TestMacros.h>

#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Assets;
using namespace TaskBar;
using namespace testing;

class TaskBarAssetProxyTest: public Test
{
public:
   TaskBarAssetProxyTest()
      : AssetClient()
      , Proxy(AssetClient)
   {
   }

   NiceMock<AssetClientMock> AssetClient;
   TaskBarAssetProxy Proxy;
};

TEST_F(TaskBarAssetProxyTest, LoadStartButtonImage1)
{
   GWT("TaskBarAssetProxy is created",
       "LoadStartButtonImage is called on default image and image is not preloaded",
       "Image load is requested");

   const Assets::ImageName img = Assets::ImageName::StartMenuImage;
   const QString imgStr = ToAssetPath(img);
   EXPECT_CALL(AssetClient, LoadImage(imgStr));

   Proxy.LoadStartButtonImage(imgStr);
}

TEST_F(TaskBarAssetProxyTest, LoadStartButtonImage2)
{
   GWT("TaskBarAssetProxy is created",
       "LoadStartButtonImage is called on default image and image is preloaded",
       "StartButtonImageReady signal is emit immediately");

   const Assets::ImageName img = Assets::ImageName::StartMenuImage;
   const QString imgStr = ToAssetPath(img);
   ON_CALL(AssetClient, IsAssetAvailable(imgStr)).WillByDefault(Return(true));
   EXPECT_CALL(AssetClient, LoadImage(imgStr)).Times(0);

   QSignalSpy spy(&Proxy, &TaskBarAssetProxy::StartButtonImageReady);

   ON_CALL(AssetClient, GetImage(imgStr)).WillByDefault(Return(QPixmap()));
   Proxy.LoadStartButtonImage(imgStr);
   ASSERT_EQ(1, spy.count());
   EXPECT_EQ(1, spy[0].count());
}

TEST_F(TaskBarAssetProxyTest, HandleImageReady1)
{
   GWT("A call to LoadImage was made",
       "ImageReady signal is received with a path equal to the requested image",
       "StartButtonImageReady is emit");
   const QString imgStr = "someString";
   Proxy.LoadStartButtonImage(imgStr);

   QSignalSpy spy(&Proxy, &TaskBarAssetProxy::StartButtonImageReady);
   AssetClient.EmitImageReady(imgStr, QPixmap());

   EXPECT_EQ(1, spy.count());
}

TEST_F(TaskBarAssetProxyTest, HandleImageReady2)
{
   GWT("A call to LoadImage was made",
       "ImageReady signal is received with a path inequal to the requested image",
       "StartButtonImageReady is not emit");
   const QString imgStr = "someString";
   Proxy.LoadStartButtonImage(imgStr);

   QSignalSpy spy(&Proxy, &TaskBarAssetProxy::StartButtonImageReady);
   AssetClient.EmitImageReady("somethingTotallyDifferent", QPixmap());

   EXPECT_EQ(0, spy.count());
}
