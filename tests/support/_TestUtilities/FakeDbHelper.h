#pragma once

#include <Settings.h>
#include <SettingsDbHelpers.h>
#include <QSqlDatabase>

class FakeDbHelper {
public:
   FakeDbHelper();
   ~FakeDbHelper() = default;

   bool SetupSystemSettingsSchema();
   bool InsertSystemSetting(Settings::Setting key, const QVariant& value);
   bool ReadSystemSetting(Settings::Setting key, const QVariant& value);
   void SeedSystemSettings(const QMap<Settings::Setting, QString>& data);
};
