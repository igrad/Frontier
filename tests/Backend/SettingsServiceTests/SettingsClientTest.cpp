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
   GWT("Service and client are created",
       "Trying to subscribe to a setting without an appropriate handler slot",
       "Subscription fails");

   FakeSettingSubscriber sub;

   EXPECT_FALSE(Client.SubscribeToSetting(Setting::WallpaperSchedule, &sub));
}

TEST_F(SettingsClientTest, SubscribeToSetting2)
{
   GWT("Service and client are created",
       "Trying to subscribe to a nullptr",
       "Subscription fails");

   EXPECT_FALSE(Client.SubscribeToSetting(Setting::TestSetting, nullptr));
}

TEST_F(SettingsClientTest, SubscribeToSetting3)
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

TEST_F(SettingsClientTest, WriteSettingValue1)
{
   GWT("Service and client are created AND setting is not None",
       "WriteSettingValue is called",
       "Emit CacheSettingValue signal");
   QSignalSpy spy(&Client, &SettingsClientInterface::CacheSettingValue);

   const Setting setting = Setting::TestSetting;
   const QVariant value = "SomeValue";
   Client.WriteSettingValue(setting, value);

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy.at(0).count());
   EXPECT_EQ(setting, spy.at(0).at(0).value<Setting>());
   EXPECT_EQ(value, spy.at(0).at(1).value<QVariant>());
}
