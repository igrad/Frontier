#include "FakeDbHelper.h"

#include <SettingsDbHelpers.h>

#include <QSqlDatabase>
#include <QSqlQuery>

using namespace Settings;

namespace
{
   constexpr const char* QUERY_BUILD_SYSTEM_SETTINGS_TABLE =
      "CREATE TABLE system_settings(setting STRING PRIMARY KEY, "
      "value STRING)";
}

FakeDbHelper::FakeDbHelper()
{
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",
                                               CONNECTION_NAME);
   db.setDatabaseName(":memory:");
   db.open();
}

bool FakeDbHelper::SetupSystemSettingsSchema()
{
   QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
   db.setDatabaseName(":memory:");
   if(!db.open())
   {
      return false;
   }

   QSqlQuery query(db);
   return query.exec(QUERY_BUILD_SYSTEM_SETTINGS_TABLE);
}

bool FakeDbHelper::InsertSystemSetting(Setting key, const QVariant& value)
{
   const QString keyStr = Settings::ToString(key);
   QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
   db.setDatabaseName(":memory:");

   QSqlQuery query(db);
   query.prepare("INSERT INTO system_settings (setting, value) VALUES (:setting, :value)");
   query.bindValue(":setting", keyStr);
   query.bindValue(":value", Settings::ToSettingString(value).c_str());
   assert(db.isOpen() && "Db isn't open");
   return query.exec();
}

bool FakeDbHelper::ReadSystemSetting(Setting key, const QVariant& value)
{
   const QString keyStr = Settings::ToString(key);
   QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
   db.setDatabaseName(":memory:");

   QSqlQuery query(db);
   query.prepare("SELECT value FROM system_settings WHERE setting = :setting LIMIT 1");
   query.bindValue(":setting", keyStr);
   assert(db.isOpen() && "Db isn't open");
   return query.exec();
}

void FakeDbHelper::SeedSystemSettings(const QMap<Settings::Setting, QString>& data)
{
   for(auto it = data.constBegin(); it != data.constEnd(); ++it)
   {
      InsertSystemSetting(it.key(), it.value());
   }
}
