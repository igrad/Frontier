#include "SettingsService.h"
#include "SettingsClient.h"

#include <ArgParser.h>
#include <Log.h>

#include <QDir>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QStandardPaths>
#include <QVariant>
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

   const std::string ToSettingString(const QVariant& val)
   {
      QString str("\"::" + val.toString() + "::\"");
      return str.toStdString();
   };

   const QVariant FromSettingString(const QString& val)
   {
      QStringList tokens = val.split("::");
      if(tokens.size() != 3)
      {
         LogError(QString("Failed to properly tokenize setting string: \"%1\"")
                     .arg(val));
         return "";
      }

      return tokens[1];
   }
}

SettingsService::SettingsService()
   : SettingsDb(QSqlDatabase::addDatabase("QSQLITE"))
{
   SetUpSettingsDatabase();
   // TODO: Set a database version number, and implement migration logic
}

SettingsService::~SettingsService()
{
   SettingsDb.close();
}

void SettingsService::SetPointerInClientClass()
{
   SettingsClient::SettingsService = this;
}

void SettingsService::SetUpSettingsDatabase()
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
      QFile SettingsDbFile(SettingsDbPath);
      if(SettingsDbFile.exists())
      {
         LogInfo("Deleting SettingsDb file");
         SettingsDbFile.remove();
      }
   }

   LogInfo("SettingsDb at " + QString::fromStdString(SettingsDbPath.generic_string()));
   SettingsDb.setDatabaseName(
      QString::fromStdString(SettingsDbPath.generic_string()));

   SettingsDb.open();

   ValidateSystemSettingsTableExists();
}

void SettingsService::ValidateSystemSettingsTableExists()
{
   if(OpenDb())
   {
      // Insert any tables necessary after building
      // Only need to do this until python build files are up and running
      QSqlQuery query;
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

      CloseDb();
   }
}

bool SettingsService::OpenDb()
{
   const bool opened = SettingsDb.open();

   if(!opened)
   {
      LogWarn("Failed to open settings database at \"" +
              QString::fromStdString(SettingsDbPath.generic_string()) +
              "\" because of error: " + SettingsDb.lastError().text());
   }

   return opened;
}

void SettingsService::CloseDb()
{
   SettingsDb.close();
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

   if(OpenDb())
   {
      QSqlQuery query;
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

      CloseDb();
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
   // QSqlQuery query;
   if(OpenDb())
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

      CloseDb();
   }

   emit SettingUpdated(setting, val);
}
