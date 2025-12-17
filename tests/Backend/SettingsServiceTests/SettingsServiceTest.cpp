#include <SettingsService.h>
#include <SettingsDbHelpers.h>

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
   FakeDbHelper(const QString& connectionName = "TestHelperConnection")
      : ConnectionName(connectionName)
   {
   }

   ~FakeDbHelper()
   {
      QSqlDatabase::removeDatabase(ConnectionName);
   }

   bool SetupSchema()
   {
      QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", ConnectionName);
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
      QSqlDatabase db = QSqlDatabase::database(ConnectionName);
      QSqlQuery query(db);
      query.prepare("INSERT INTO system_settings (setting, value) VALUES (:setting, :value)");
      query.bindValue(":setting", keyStr);
      query.bindValue(":value", value);
      return query.exec();
   }

   bool ReadSystemSetting(Setting key, const QVariant& value)
   {
      const QString keyStr = Settings::ToString(key);
      QSqlDatabase db = QSqlDatabase::database(ConnectionName);
      QSqlQuery query(db);
      query.prepare("SELECT value FROM system_settings WHERE setting = :setting LIMIT 1");
      query.bindValue(":setting", keyStr);
      return query.exec();
   }

   void SeedSystemSettings(const QMap<Settings::Setting, QString>& data)
   {
      for(auto it = data.constBegin(); it != data.constEnd(); ++it)
      {
         InsertSystemSetting(it.key(), it.value());
      }
   }

private:
   QString ConnectionName;
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
}

TEST(SettingsServiceTest, FetchAllSettings3)
{
   GWT("A db has one setting set",
       "FetchAllSettings is called",
       "Emit SettingUpdated once")
   FakeDbHelper db;
   SettingsService service;

   db.SetupSchema();

   QSignalSpy spy(&service, &SettingsService::SettingUpdated);

   EXPECT_TRUE(false);
}
