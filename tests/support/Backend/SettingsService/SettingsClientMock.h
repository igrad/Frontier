#pragma once

#include <SettingsClientInterface.h>

#include <gmock/gmock.h>

class SettingsClientMock: public SettingsClientInterface
{
public:
   MOCK_METHOD(bool, SubscribeToSetting, (Settings::Setting, QObject*), (override));
   MOCK_METHOD(bool, WriteSettingValue, (Settings::Setting, const QVariant&), (override));
};
