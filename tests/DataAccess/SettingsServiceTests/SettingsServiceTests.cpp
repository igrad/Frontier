#include <SettingsClient.h>
#include <SettingsDbHelpers.h>
#include <SettingsService.h>

#include <SettingsService/SettingsClientMock.h>
#include <TestMacros.h>
#include <FakeDbHelper.h>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QSignalSpy>
#include <QLoggingCategory>

using namespace Settings;
using namespace testing;

namespace
{
   void MuteQtSqlWarnings()
   {
      QLoggingCategory::setFilterRules("qt.sql.warning=false");
   }

   void UnmuteQtSqlWarnings()
   {
      QLoggingCategory::setFilterRules("qt.sql.warning=true");
   }
}

TEST(SettingsServiceTest, FetchAllSettings1)
{
   GWT("An unconfigured empty test database",
       "FetchAllSettings is called",
       "Do not emit SettingUpdated signal")
   FakeDbHelper db;

   MuteQtSqlWarnings();
   {
      SettingsService service;

      QSignalSpy spy(&service, &SettingsService::SettingUpdated);

      service.FetchAllSettings();

      EXPECT_EQ(0, spy.count());
   }
   UnmuteQtSqlWarnings();
}

TEST(SettingsServiceTest, FetchAllSettings2)
{
   GWT("A configured empty test database",
       "FetchAllSettings is called",
       "Do not emit SettingUpdated signal")
   FakeDbHelper db;
   SettingsService service;

   db.SetupSystemSettingsSchema();

   QSignalSpy spy(&service, &SettingsService::SettingUpdated);

   service.FetchAllSettings();

   EXPECT_EQ(0, spy.count());
}

TEST(SettingsServiceTest, FetchAllSettings3)
{
   GWT("A db has one setting set",
       "FetchAllSettings is called",
       "Emit SettingUpdated once")
   FakeDbHelper db;
   SettingsService service;

   db.SetupSystemSettingsSchema();
   const QVariant value("SomeValue");
   db.InsertSystemSetting(Setting::_TestSetting, value);

   QSignalSpy spy(&service, &SettingsService::SettingUpdated);

   service.FetchAllSettings();

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy.at(0).count());
   EXPECT_EQ(Setting::_TestSetting, spy.at(0).at(0).value<Setting>());
   EXPECT_EQ(value, spy.at(0).at(1).value<QVariant>());
}

TEST(SettingsServiceTest, SetPointerInClientClass1)
{
   GWT("The SettingsService has been created",
       "The SettingsService is destroyed",
       "The SettingsClient::SettingsService pointer becomes null");
   {
      SettingsService svc;
      EXPECT_NE(nullptr, SettingsClient::GetSettingsServicePtr());
   }

   EXPECT_EQ(nullptr, SettingsClient::GetSettingsServicePtr());
}

TEST(SettingsServiceTest, HandleWriteSettingValueTest1)
{
   GWT("The program is already running",
       "One component writes a new settings value to disk",
       "Other subscribers to that setting are notified");

   SettingsService service;

   QSignalSpy spy(&service, &SettingsService::SettingUpdated);

   const QVariant value("SomeValue");
   service.HandleCacheSettingValue(Setting::_TestSetting,
                                   value);

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy.at(0).count());
   EXPECT_EQ(Setting::_TestSetting, spy.at(0).at(0).value<Setting>());
   EXPECT_EQ(value, spy.at(0).at(1).value<QVariant>());
}
