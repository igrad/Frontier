#include "SettingsService.h"
#include "SettingsClient.h"
#include "SettingsDbHelpers.h"

#include <ArgParser.h>
#include <Log.h>

#include <QDir>
#include <QtSql/QSqlError>
#include <QStandardPaths>
#include <cassert>

using namespace Settings;

namespace
{
   constexpr const char* SETTINGS_DB_NAME = "FrontierSettings.db";

   constexpr const char* QUERY_WRITE_SYSTEM_SETTING =
      "INSERT OR REPLACE INTO system_settings (setting, value) "
      "VALUES (:setting, :value);";
   constexpr const char* QUERY_WRITE_DISPLAY_SETTING =
      "INSERT OR REPLACE INTO display_settings (setting, display, value) "
      "VALUES (:setting, :display, :value);";
   constexpr const char* QUERY_READ_SYSTEM_SETTING =
      "SELECT value FROM system_settings "
      "WHERE setting = :setting "
      "LIMIT 1;";
   constexpr const char* QUERY_READ_DISPLAY_SETTING =
      "SELECT value FROM system_settings "
      "WHERE setting = :setting "
      "AND display = :display "
      "LIMIT 1;";
   constexpr const char* QUERY_READ_ALL_SYSTEM_SETTINGS =
      "SELECT * FROM system_settings;";
   constexpr const char* QUERY_CHECK_IF_SYSTEM_SETTINGS_EXISTS =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='system_settings';";
   constexpr const char* QUERY_CHECK_IF_DISPLAY_SETTINGS_EXISTS =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='display_settings';";

   // TODO: This is fine for now, but eventually we need to migrate to using
   // versioned python files for db inits/migrations
   constexpr const char* QUERY_BUILD_SYSTEM_SETTINGS_TABLE =
      "CREATE TABLE system_settings(setting STRING PRIMARY KEY, "
      "value STRING)";
   constexpr const char* QUERY_BUILD_DISPLAY_SETTINGS_TABLE =
      "CREATE TABLE display_settings(setting STRING PRIMARY KEY, "
      "display INT, "
      "value STRING)";
}

bool SettingsService::UseRAMDatabases = false;

SettingsService::SettingsService(QObject* parent)
   : SettingsDbPath("")
   , DatabaseName("")
{
   setParent(parent);

   SetUpSettingsDatabase();
   SetPointerInClientClass();
   // TODO: Set a database version number, and implement migration logic
}

SettingsService::~SettingsService()
{
   QSqlDatabase::removeDatabase(CONNECTION_NAME);
   SettingsClient::Service = nullptr;
}

void SettingsService::FetchAllSettings()
{
   // TODO: This could probably be optimized
   // Check out QSqlQuery::BatchExecutionMode enum
   QMap<Setting, QVariant> fetchedValues;

   QSqlDatabase db = GetDb();
   if(!OpenDb(db))
   {
      LogWarn("Failed to open database!");
      return;
   }

   SqlQuery query(db);
   query.prepare(QUERY_READ_ALL_SYSTEM_SETTINGS);
   query.setForwardOnly(true);

   if(RunQuery(query))
   {
      while(query.next())
      {
         const Setting setting = ToSetting(query.value(0).toString());
         const QVariant val = FromSettingString(query.value(1).toString());
         fetchedValues[setting] = val;
      }
   }

   // Run the emits after the database is closed since these are more likely to cause any issues
   LogInfo(QString("Fetched %1 settings from system_settings").arg(fetchedValues.count()))
   for(const QPair<Setting, QVariant>& pair : fetchedValues.asKeyValueRange())
   {
      emit SystemSettingUpdated(pair.first, pair.second);
   }
}

void SettingsService::HandleCacheSystemSettingValue(const Setting setting, const QVariant val)
{
   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      SqlQuery query(db);
      query.prepare(QUERY_WRITE_SYSTEM_SETTING);
      query.bindValue(":setting", ToString(setting));
      query.bindValue(":value", ToSettingString(val).c_str());

      if(RunQuery(query) && (query.numRowsAffected() != 1))
      {
         LogWarn(QString("Settings update should have only affected 1 row, "
                         "but it affected %1 rows").arg(query.numRowsAffected()))
      }
   }

   emit SystemSettingUpdated(setting, val);
}

void SettingsService::HandleCacheDisplaySettingValue(const Setting setting,
                                                     uint8_t display,
                                                     const QVariant val)
{
   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      SqlQuery query(db);
      query.prepare(QUERY_WRITE_DISPLAY_SETTING);
      query.bindValue(":setting", ToString(setting));
      query.bindValue(":display", display);
      query.bindValue(":value", ToSettingString(val).c_str());

      if(RunQuery(query) && (query.numRowsAffected() != 1))
      {
         LogWarn(QString("Settings update should have only affected 1 row, "
                         "but it affected %1 rows").arg(query.numRowsAffected()))
      }
   }

   emit DisplaySettingUpdated(setting, display, val);
}

void SettingsService::HandleRequestSystemSettingValue(const Setting setting)
{
   QSqlDatabase db = GetDb();
   QVariant val;
   if(OpenDb(db))
   {
      SqlQuery query(db);
      query.prepare(QUERY_READ_SYSTEM_SETTING);
      query.bindValue(":setting", ToString(setting));
      query.setForwardOnly(true);

      if(RunQuery(query) && query.next())
      {
         val = FromSettingString(query.value(1).toString());
      }
   }

   emit SystemSettingUpdated(setting, val);
}

void SettingsService::HandleRequestDisplaySettingValue(const Setting setting, uint8_t display)
{
   QSqlDatabase db = GetDb();
   QVariant val;
   if(OpenDb(db))
   {
      SqlQuery query(db);
      query.prepare(QUERY_READ_DISPLAY_SETTING);
      query.bindValue(":setting", ToString(setting));
      query.bindValue(":display", display);
      query.setForwardOnly(true);

      if(RunQuery(query) && query.next())
      {
         val = FromSettingString(query.value(1).toString());
      }
   }

   emit SystemSettingUpdated(setting, val);
}

void SettingsService::SetPointerInClientClass()
{
   SettingsClient::Service = this;
}

void SettingsService::SetUpSettingsDatabase()
{
   QSqlDatabase db;
   if(UseRAMDatabases || ArgParser::RunningUnitTests())
   {
      // This chunk is for unit testing and Enterprise purposes
      // :memory: specifies that the database will simply be held in RAM
      // instead of in a file on-disk.
      SettingsDbPath = ":memory:";

      const std::string pathStr = SettingsDbPath.generic_string();
      if(QSqlDatabase::contains(CONNECTION_NAME))
      {
         db = QSqlDatabase::database(CONNECTION_NAME);
      }
      else
      {
         db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
      }
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

   ValidateSystemSettingsTableExists();
   ValidateDisplaySettingsTableExists();
}

void SettingsService::ValidateSystemSettingsTableExists()
{
   LogInfo("Validating SystemSettingsTable exists");

   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      // Insert any tables necessary after building
      // Only need to do this until python build files are up and running
      SqlQuery query(db);
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

void SettingsService::ValidateDisplaySettingsTableExists()
{
   LogInfo("Validating DisplaySettingsTable exists");

   QSqlDatabase db = GetDb();
   if(OpenDb(db))
   {
      // Insert any tables necessary after building
      // Only need to do this until python build files are up and running
      SqlQuery query(db);
      LogInfo(QUERY_CHECK_IF_DISPLAY_SETTINGS_EXISTS)
      query.prepare(QString(QUERY_CHECK_IF_DISPLAY_SETTINGS_EXISTS));
      query.setForwardOnly(true);
      if(RunQuery(query) && !query.next())
      {
         // Table doesn't exist
         query.clear();
         query.prepare(QUERY_BUILD_DISPLAY_SETTINGS_TABLE);
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

bool SettingsService::RunQuery(SqlQuery& query)
{
   if(!query.exec())
   {
      const QSqlError err = query.lastError();
      LogError(QString("Unable to run query against settings database at \"%1"
                      "\" because of error: %2")
                 .arg(QString::fromStdString(SettingsDbPath.generic_string()),
                      err.text()));

      if(QSqlError::ConnectionError == err.type())
      {
         LogError("Connection error. Check that the database has been set in your query!");
      }

      return false;
   }

   return true;
}
