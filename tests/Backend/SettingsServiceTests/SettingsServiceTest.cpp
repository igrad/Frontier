#include <SettingsService.h>

#include <SettingsService/SettingsClientMock.h>
#include <_Utilities/TestMacros.h>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <gtest/gtest.h>

using namespace testing;

namespace
{
   constexpr const char* QUERY_BUILD_SYSTEM_SETTINGS_TABLE =
      "CREATE TABLE system_settings(setting STRING PRIMARY KEY, "
      "value STRING)";
}

class FakeDbHelper {
public:
   FakeDbHelper(const QString& connectionName = "TestHelperConnection")
      : ConnectionName(connectionName)
   {
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

   bool InsertSystemSetting(Setting key, const QString& value)
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
      query.prepare("SELECT value FROM system_settings (setting, value) VALUES (:setting, :value)");
      query.bindValue(":setting", keyStr);
      query.bindValue(":value", value);
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

TEST(SettingsServiceTest, FetchAllSettingsNegative)
{
   SCOPED_TRACE("GIVEN: No settings present in db\n"
                "WHEN: FetchAllSettings is called\n"
                "THEN: Do no emit SettingUpdated signal");

}
