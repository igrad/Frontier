#include <SettingsClient.h>
#include <SettingsDbHelpers.h>
#include <SettingsService.h>

#include <SettingsService/SettingsClientMock.h>
#include <_Utilities/TestMacros.h>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QSignalSpy>
#include <QLoggingCategory>

using namespace testing;

namespace
{
   constexpr const char* QUERY_BUILD_SYSTEM_SETTINGS_TABLE =
      "CREATE TABLE system_settings(setting STRING PRIMARY KEY, "
      "value STRING)";

   void MuteQtSqlWarnings()
   {
      QLoggingCategory::setFilterRules("qt.sql.warning=false");
   }

   void UnmuteQtSqlWarnings()
   {
      QLoggingCategory::setFilterRules("qt.sql.warning=true");
   }
}

class FakeDbHelper {
public:
   FakeDbHelper()
   {
      QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",
                                                  Settings::CONNECTION_NAME);
      db.setDatabaseName(":memory:");
      db.open();
   }

   ~FakeDbHelper() = default;

   bool SetupSchema()
   {
      QSqlDatabase db = QSqlDatabase::database(Settings::CONNECTION_NAME);
      db.setDatabaseName(":memory:");
      if(!db.open())
      {
         return false;
      }

      QSqlQuery query(db);
      return query.exec(QUERY_BUILD_SYSTEM_SETTINGS_TABLE);
   }

   bool InsertSystemSetting(Setting key, const QVariant& value)
   {
      const QString keyStr = Settings::ToString(key);
      QSqlDatabase db = QSqlDatabase::database(Settings::CONNECTION_NAME);
      db.setDatabaseName(":memory:");
      QSqlQuery query(db);
      query.prepare("INSERT INTO system_settings (setting, value) VALUES (:setting, :value)");
      query.bindValue(":setting", keyStr);
      query.bindValue(":value", Settings::ToSettingString(value).c_str());
      assert(db.isOpen() && "Db isn't open");
      return query.exec();
   }

   bool ReadSystemSetting(Setting key, const QVariant& value)
   {
      const QString keyStr = Settings::ToString(key);
      QSqlDatabase db = QSqlDatabase::database(Settings::CONNECTION_NAME);
      db.setDatabaseName(":memory:");
      QSqlQuery query(db);
      query.prepare("SELECT value FROM system_settings WHERE setting = :setting LIMIT 1");
      query.bindValue(":setting", keyStr);
      assert(db.isOpen() && "Db isn't open");
      return query.exec();
   }

   void SeedSystemSettings(const QMap<Settings::Setting, QString>& data)
   {
      for(auto it = data.constBegin(); it != data.constEnd(); ++it)
      {
         InsertSystemSetting(it.key(), it.value());
      }
   }
};

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

   db.SetupSchema();

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

   db.SetupSchema();
   const QVariant value("SomeValue");
   db.InsertSystemSetting(Setting::WallpaperSchedule, value);

   QSignalSpy spy(&service, &SettingsService::SettingUpdated);

   service.FetchAllSettings();

   ASSERT_EQ(1, spy.count());
   ASSERT_EQ(2, spy.at(0).count());
   EXPECT_EQ(Setting::WallpaperSchedule, spy.at(0).at(0).value<Setting>());
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
