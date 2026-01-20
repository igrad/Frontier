#pragma once

#include "SettingsServiceInterface.h"

#include <QtSql/QSqlDatabase>
#include <Sql/SqlQuery.h>

#include <filesystem>

namespace Settings
{
   class SettingsService: public SettingsServiceInterface
   {
      Q_OBJECT

   public:
      explicit SettingsService(QObject* parent = nullptr);
      ~SettingsService();

      void FetchAllSettings() override;

      static bool UseRAMDatabases;

   public slots:
      void HandleCacheSystemSettingValue(const Setting setting,
                                         const QVariant val) override;
      void HandleCacheDisplaySettingValue(const Setting setting,
                                          const QString& displayID,
                                          const QVariant value) override;
      void HandleRequestSystemSettingValue(const Setting setting) override;
      void HandleRequestDisplaySettingValue(const Setting setting,
                                            const QString& displayID) override;

   private:
      void SetPointerInClientClass();
      void SetUpSettingsDatabase();
      void ValidateSystemSettingsTableExists();
      void ValidateDisplaySettingsTableExists();

      inline QSqlDatabase GetDb();
      bool OpenDb(QSqlDatabase& db);
      bool RunQuery(SqlQuery& query);
      void WriteSystemSettingToDatabase(const Setting setting, const QVariant val);

      std::filesystem::path SettingsDbPath;
      QString DatabaseName;
   };
}
