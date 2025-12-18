#include "SettingsService.h"
#include "SettingsClient.h"
#include "SettingsDbHelpers.h"

#include <ArgParser.h>
#include <Log.h>

#include <QDir>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QStandardPaths>
#include <cassert>

using namespace Settings;

namespace
{
   constexpr const char* SETTINGS_DB_NAME = "FrontierSettings.db";

   constexpr const char* QUERY_WRITE_SYSTEM_SETTING =
      "INSERT INTO system_settings (setting, value) "
      "VALUES (:setting, :value);";
   constexpr const char* QUERY_READ_SYSTEM_SETTING =
      "SELECT value FROM system_settings "
      "WHERE setting = :setting "
      "LIMIT 1;";
   constexpr const char* QUERY_READ_ALL_SYSTEM_SETTINGS =
      "SELECT * FROM system_settings;";
   constexpr const char* QUERY_CHECK_IF_SYSTEM_SETTINGS_EXISTS =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='system_settings';";

   // TODO: This is fine for now, but eventually we need to migrate to using
   // versioned python files for db inits/migrations
   constexpr const char* QUERY_BUILD_SYSTEM_SETTINGS_TABLE =
      "CREATE TABLE system_settings(setting STRING PRIMARY KEY, "
      "value STRING)";
}

SettingsService::SettingsService()
   : SettingsDbPath("")
   , DatabaseName("")
{
   SetUpSettingsDatabase();
   // TODO: Set a database version number, and implement migration logic
}

SettingsService::~SettingsService()
{
   QSqlDatabase::removeDatabase(CONNECTION_NAME);
}

void SettingsService::SetPointerInClientClass()
{
   SettingsClient::SettingsService = this;
}

void SettingsService::SetUpSettingsDatabase()
{
   QSqlDatabase db;
   if(ArgParser::RunningUnitTests())
   {
      SettingsDbPath = ":memory:";

      const std::string pathStr = SettingsDbPath.generic_string();
      db = QSqlDatabase::database(CONNECTION_NAME);
   }
   else
   {
      // Find appdata folder
      QStringList locations =
         QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);

      // TODO: What do we do if a writeable location cannot be found?
      // Docs say that the first result from standardLocations will be the
      // writableLocation
      assert(0 < locations.count());
      QString appDataDir =
         QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

      assert(!appDataDir.trimmed().isEmpty() &&
             "Could not find AppLocalDataLocation to write to!");

      SettingsDbPath = appDataDir.toStdString() + "\\" + SETTINGS_DB_NAME;
      if(ArgParser::RunningInCleanMode())
      {
         QFile settingsDbFile(SettingsDbPath);
         if(settingsDbFile.exists())
         {
            LogInfo("Deleting SettingsDb file");
            settingsDbFile.remove();
         }
      }

      const std::string pathStr = SettingsDbPath.generic_string();
      LogInfo("SettingsDb at " + QString::fromStdString(pathStr));
      db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
   }

   DatabaseName = QString::fromStdString(SettingsDbPath.generic_string());
   db.setDatabaseName(DatabaseName);
   db.open();
   if(OpenDb(db))
   {
      ValidateSystemSettingsTableExists();
   }
}

void SettingsService::ValidateSystemSettingsTableExists()
{
   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      // Insert any tables necessary after building
      // Only need to do this until python build files are up and running
      QSqlQuery query(db);
      LogInfo(QUERY_CHECK_IF_SYSTEM_SETTINGS_EXISTS)
      query.prepare(QString(QUERY_CHECK_IF_SYSTEM_SETTINGS_EXISTS));
      query.setForwardOnly(true);
      if(RunQuery(query) && !query.next())
      {
         // Table doesn't exist
         query.clear();
         query.prepare(QUERY_BUILD_SYSTEM_SETTINGS_TABLE);
         query.setForwardOnly(false);
         RunQuery(query);
      }
   }
}

QSqlDatabase SettingsService::GetDb()
{
   QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
   db.setDatabaseName(DatabaseName);
   return db;
}

bool SettingsService::OpenDb(QSqlDatabase& db)
{
   const bool open = db.isOpen();
   if(!open)
   {
      LogWarn("Failed to open settings database at \"" +
              QString::fromStdString(SettingsDbPath.generic_string()) +
              "\" because of error: " + db.lastError().text());
   }

   return open;
}

bool SettingsService::RunQuery(QSqlQuery& query)
{
   if(!query.exec())
   {
      LogWarn("Unable to run query against settings database at \"" +
              QString::fromStdString(SettingsDbPath.generic_string()) +
              "\" because of error: " + query.lastError().text());
      return false;
   }

   return true;
}

void SettingsService::FetchAllSettings()
{
   // TODO: This could probably be optimized
   QMap<Setting, QVariant> fetchedValues;

   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      QSqlQuery query(db);
      query.prepare(QUERY_READ_ALL_SYSTEM_SETTINGS);
      query.setForwardOnly(true);

      if(RunQuery(query))
      {
         while(query.next())
         {
            const QString settingStr = query.value(0).toString();
            const Setting setting = FromString(settingStr);

            if(Setting::None != setting)
            {
               const QVariant val = FromSettingString(query.value(1).toString());
               fetchedValues[setting] = val;
            }
         }
      }
   }

   // Run the emits after the database is closed since these are more likely to cause any issues
   LogInfo(QString("Fetched %1 settings from system_settings").arg(fetchedValues.count()))
   for(const QPair<Setting, QVariant>& pair : fetchedValues.asKeyValueRange())
   {
      emit SettingUpdated(pair.first, pair.second);
   }
}

void SettingsService::HandleWriteSettingValue(const Setting setting, const QVariant val)
{
   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      QSqlQuery query;
      query.prepare(QUERY_WRITE_SYSTEM_SETTING);
      query.bindValue(":setting", ToString(setting));
      query.bindValue(":value", ToSettingString(val).c_str());

      if(RunQuery(query) && (query.numRowsAffected() != 1))
      {
         LogWarn(QString("Settings update should have only affected 1 row, "
                         "but it affected %1 rows").arg(query.numRowsAffected()))
      }
   }

   emit SettingUpdated(setting, val);
}
