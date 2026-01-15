#pragma once

#include <SettingsServiceInterface.h>

#include <gmock/gmock.h>

class SettingsServiceMock: public Settings::SettingsServiceInterface
{
public:
   void EmitSystemSettingUpdated(const Settings::Setting setting,
                                 const QVariant& val);
   void EmitDisplaySettingUpdated(const Settings::Setting setting,
                                  uint8_t display,
                                  const QVariant& val);

   MOCK_METHOD(void, FetchAllSettings, (), (override));

   // Slots
   MOCK_METHOD(void,
               HandleCacheSystemSettingValue,
               (const Settings::Setting setting, const QVariant value),
               (override));
   MOCK_METHOD(void,
               HandleCacheDisplaySettingValue,
               (const Settings::Setting setting, uint8_t display, const QVariant value),
               (override));
   MOCK_METHOD(void,
               HandleRequestSystemSettingValue,
               (const Settings::Setting setting),
               (override));
   MOCK_METHOD(void,
               HandleRequestDisplaySettingValue,
               (const Settings::Setting setting, uint8_t display),
               (override));
};
