#pragma once

#include "SettingsServiceInterface.h"

#include <QtSql/QSqlDatabase>

#include <filesystem>

namespace Settings
{
   class SettingsService: public SettingsServiceInterface
   {
      Q_OBJECT

   public:
      SettingsService();
      ~SettingsService();

      void FetchAllSettings() override;

   public slots:
      void HandleCacheSettingValue(const Setting setting,
                                   const QVariant val) override;

   private:
      void SetPointerInClientClass();
      void SetUpSettingsDatabase();
      void ValidateSystemSettingsTableExists();

      inline QSqlDatabase GetDb();
      bool OpenDb(QSqlDatabase& db);
      bool RunQuery(QSqlQuery& query);
      void WriteSettingToDatabase(const Setting setting, const QVariant val);

      std::filesystem::path SettingsDbPath;
      QString DatabaseName;
   };
}
