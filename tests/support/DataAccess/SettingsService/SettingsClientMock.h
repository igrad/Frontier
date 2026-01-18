#pragma once

#include <SettingsClientInterface.h>

#include <gmock/gmock.h>

class SettingsClientMock: public Settings::SettingsClientInterface
{
public:
   void EmitCacheSystemSettingValue(Settings::Setting setting,
                                    const QVariant& value);
   void EmitCacheDisplaySettingValue(Settings::Setting setting,
                                     const QString& displayID,
                                     const QVariant& value);

   MOCK_METHOD(bool, SubscribeToSystemSetting, (Settings::Setting,
                                                QObject*), (override));
   MOCK_METHOD(bool, SubscribeToDisplaySetting, (Settings::Setting,
                                                 QObject*), (override));
   MOCK_METHOD(bool, SubscribeToAllSystemSettings, (QObject*), (override));
   MOCK_METHOD(bool, SubscribeToAllDisplaySettings, (QObject*), (override));
   MOCK_METHOD(bool, WriteSystemSettingValue, (Settings::Setting,
                                               const QVariant&), (override));
   MOCK_METHOD(bool, WriteDisplaySettingValue, (Settings::Setting,
                                                const QString&,
                                                const QVariant&), (override));
};
