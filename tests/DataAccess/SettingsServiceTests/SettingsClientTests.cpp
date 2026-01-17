#include <SettingsClient.h>
#include <SettingsService/SettingsServiceMock.h>
#include <SettingsService/SettingsServicePointerHelper.h>

#include <FakeSettingSubscriber.h>
#include <TestMacros.h>

#include <QSignalSpy>
#include <QTestEventLoop>

using namespace Settings;
using namespace testing;

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
   SettingsServicePointerHelper PtrHelper;
   SettingsClient Client;
};

TEST_F(SettingsClientTest, SubscribeToSystemSetting1)
{
   GWT("Service and client are created",
       "Trying to subscribe to a setting without an appropriate handler slot",
       "Subscription fails");

   FakeSettingSubscriber sub;

   EXPECT_FALSE(Client.SubscribeToSystemSetting(Setting::WallpaperSchedule, &sub));
}

TEST_F(SettingsClientTest, SubscribeToSystemSetting2)
{
   GWT("Service and client are created",
       "Trying to subscribe to a nullptr",
       "Subscription fails");

   EXPECT_FALSE(Client.SubscribeToSystemSetting(Setting::_TestSetting, nullptr));
}

TEST_F(SettingsClientTest, SubscribeToSystemSetting3)
{
   GWT("Service and client are created AND subscribed to setting",
       "Setting is updated by service",
       "Client receives signal and invokes sub slot");

   FakeSettingSubscriber sub;

   Client.SubscribeToSystemSetting(Setting::_TestSetting, &sub);

   QSignalSpy spy(&sub, &FakeSettingSubscriber::_TestSettingReceived);

   const QVariant value = QString("SomeValue");
   ServiceMock.EmitSystemSettingUpdated(Setting::_TestSetting, value);

   QCoreApplication::processEvents();

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(1, spy.at(0).count());
   EXPECT_EQ(value, spy.at(0).at(0).value<QVariant>());
}

TEST_F(SettingsClientTest, SubscribeToAllSystemSettings1)
{
   GWT("Service and client are created",
       "Trying to subscribe to all system settings without an appropriate handler slot",
       "Subscription fails");

   FakeSettingSubscriber sub;

   EXPECT_FALSE(Client.SubscribeToAllSystemSettings(&sub));
}

TEST_F(SettingsClientTest, SubscribeToAllSystemSettings2)
{
   GWT("Service and client are created AND subscribed to all settings",
       "Several settings are updated by service",
       "Client receives signals for each setting updated");

   FakeAllSettingsSubscriber sub;

   Client.SubscribeToAllSystemSettings(&sub);

   QSignalSpy spy(&sub, &FakeAllSettingsSubscriber::SettingChangeReceived);

   const QVariant value = QString("SomeValue");
   ServiceMock.EmitSystemSettingUpdated(Setting::_TestSetting, value);

   ASSERT_TRUE(spy.wait());

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy.at(0).count());
   EXPECT_EQ(Setting::_TestSetting, spy.at(0).at(0).value<Settings::Setting>());
   EXPECT_EQ(value, spy.at(0).at(1).value<QVariant>());

   ServiceMock.EmitSystemSettingUpdated(Setting::TaskBarAlignment, value);

   ASSERT_TRUE(spy.wait());

   ASSERT_EQ(2, spy.count());
   ASSERT_EQ(2, spy.at(1).count());
   EXPECT_EQ(Setting::TaskBarAlignment, spy.at(1).at(0).value<Settings::Setting>());
   EXPECT_EQ(value, spy.at(1).at(1).value<QVariant>());
}

TEST_F(SettingsClientTest, HandleSystemSettingUpdated1)
{
   GWT("Service and client are created AND setting is not None",
       "WriteSystemSettingValue is called",
       "Emit CacheSystemSettingValue signal");
   QSignalSpy spy(&Client, &SettingsClientInterface::CacheSystemSettingValue);

   const Setting setting = Setting::_TestSetting;
   const QVariant value = "SomeValue";
   Client.WriteSystemSettingValue(setting, value);

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy.at(0).count());
   EXPECT_EQ(setting, spy.at(0).at(0).value<Setting>());
   EXPECT_EQ(value, spy.at(0).at(1).value<QVariant>());
}
