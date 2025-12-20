#include <SettingsClient.h>
#include <SettingsService/SettingsServiceMock.h>

#include <FakeSettingSubscriber.h>
#include <TestMacros.h>

#include <QSignalSpy>
#include <QTestEventLoop>

using namespace Settings;
using namespace testing;

struct ServicePointerHelper
{
   ServicePointerHelper(NiceMock<SettingsServiceMock>* service)
   {
      SettingsClient::Service = service;
   }
};

class SettingsClientTest: public Test
{
public:
   SettingsClientTest()
      : ServiceMock()
      , PtrHelper(&ServiceMock)
      , Client("SettingsClientOwner")
   {
   }

   NiceMock<SettingsServiceMock> ServiceMock;
   ServicePointerHelper PtrHelper;
   SettingsClient Client;
};

TEST_F(SettingsClientTest, SubscribeToSetting1)
{
   GWT("Service and client are created AND subscribed to setting",
       "Setting is updated by service",
       "Client receives signal and invokes sub slot");

   FakeSettingSubscriber sub;

   Client.SubscribeToSetting(Setting::TestSetting, &sub);

   QSignalSpy spy(&sub, &FakeSettingSubscriber::TestSettingReceived);

   const QVariant value = QString("SomeValue");
   ServiceMock.EmitSettingUpdated(Setting::TestSetting, value);

   ASSERT_TRUE(spy.wait());

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(1, spy.at(0).count());
   EXPECT_EQ(value, spy.at(0).at(0).value<QVariant>());
}
