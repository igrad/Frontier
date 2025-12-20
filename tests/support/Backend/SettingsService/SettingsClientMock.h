#pragma once

#include <SettingsClientInterface.h>

#include <gmock/gmock.h>

class SettingsClientMock: public SettingsClientInterface
{
public:
   MOCK_METHOD(bool, SubscribeToSetting, (const Settings::Setting&, QObject*), (override));
};
