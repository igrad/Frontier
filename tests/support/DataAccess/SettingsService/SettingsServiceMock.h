#pragma once

#include <SettingsServiceInterface.h>

#include <gmock/gmock.h>

class SettingsServiceMock: public Settings::SettingsServiceInterface
{
public:
   void EmitSettingUpdated(const Settings::Setting setting,
                           const QVariant val)
   {
      emit SettingUpdated(setting, val);
   }

   MOCK_METHOD(void, FetchAllSettings, (), (override));

   // Slots
   MOCK_METHOD(void,
               HandleCacheSettingValue,
               (const Settings::Setting setting, const QVariant value),
               (override));
};
