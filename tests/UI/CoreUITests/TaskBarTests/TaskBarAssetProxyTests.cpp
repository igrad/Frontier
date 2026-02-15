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

   const Assets::ImageName img = Assets::ImageName::_TestName;
   const QString imgStr = ToAssetPath(img);
   ON_CALL(AssetClient, IsAssetAvailable(imgStr)).WillByDefault(Return(true));
   EXPECT_CALL(AssetClient, LoadImage(imgStr)).Times(0);

   QSignalSpy spy(&Proxy, &TaskBarAssetProxy::StartButtonImageReady);

   QPixmap testPixmap;
   testPixmap.load("://images/_TestImage.png");
   ON_CALL(AssetClient, GetImage(imgStr)).WillByDefault(Return(testPixmap));
   Proxy.LoadStartButtonImage(imgStr);
   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(1, spy[0].count());
   EXPECT_FALSE(spy[0][0].value<QPixmap>().isNull());
}
