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
      "VALUES (:setting, :value)";
   constexpr const char* QUERY_READ_SYSTEM_SETTING =
      "SELECT value FROM system_settings "
      "WHERE setting = :setting "
      "LIMIT 1";
   constexpr const char* QUERY_READ_ALL_SYSTEM_SETTINGS =
      "SELECT * FROM system_settings";
   constexpr const char* QUERY_CHECK_IF_SYSTEM_SETTINGS_EXISTS =
      "SELECT CASE WHEN OBJECT_ID('system_settings', 'U') IS NOT NULL "
      "THEN 1 ELSE 0 END AS SytemSettingsExists;";

   // TODO: This is fine for now, but eventually we need to migrate to using
   // versioned python files for db inits/migrations
   constexpr const char* QUERY_BUILD_SYSTEM_SETTINGS_TABLE =
      "CREATE TABLE system_settings(setting STRING PRIMARY KEY, "
      "value STRING)";
}

SettingsService::SettingsService()
   : SettingsDb(QSqlDatabase::addDatabase("QSQLITE"))
{
   SetUpSettingsDatabase();
   // TODO: Set a database version number, and implement migration logic
   // TODO: Per the docs, do not keep the QSqlDatabase object as a member
}

SettingsService::~SettingsService()
{
   SettingsClient::SettingsService = nullptr;
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
      if(RunQuery(query))
      {
         if(0 == query.value(0))
         {
            // Table doesn't exist
            query.clear();
            query.prepare(QUERY_BUILD_SYSTEM_SETTINGS_TABLE);
            query.setForwardOnly(false);
            RunQuery(query);
         }
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
}

void SettingsService::HandleWriteSettingValue(const Setting setting, const QVariant val)
{
   // QSqlQuery query;
}
