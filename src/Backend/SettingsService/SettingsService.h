#pragma once

#include "Settings.h"

#include <QObject>
#include <QtSql/QSqlDatabase>

#include <filesystem>

namespace Settings
{
   class SettingsService: public QObject
   {
      Q_OBJECT

   public:
      SettingsService();
      ~SettingsService();

      void FetchAllSettings();

   public slots:
      void HandleWriteSettingValue(const Setting setting, const QVariant val);

   signals:
      void SettingUpdated(const Setting setting, const QVariant val);

   private:
      void SetPointerInClientClass();
      void SetUpSettingsDatabase();
      void ValidateSystemSettingsTableExists();

      QSqlDatabase GetDb();
      bool OpenDb(QSqlDatabase& db);
      bool RunQuery(QSqlQuery& query);
      void WriteSettingToDatabase(const Setting setting, const QVariant val);

      std::filesystem::path SettingsDbPath;
   };
}
