#pragma once

#include <SettingsServiceInterface.h>

#include <gmock/gmock.h>

class SettingsServiceMock: public SettingsServiceInterface
{
public:
   void EmitSettingUpdated(const Setting setting,
                           const QVariant val)
   {
      emit SettingUpdated(setting, val);
   }

   MOCK_METHOD(void, FetchAllSettings, (), (override));

   // Slots
   MOCK_METHOD(void,
               HandleCacheSettingValue,
               (const Setting setting, const QVariant value),
               (override));
};
