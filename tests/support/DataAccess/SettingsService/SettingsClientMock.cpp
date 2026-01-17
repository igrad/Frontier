#include "SettingsClientMock.h"

using namespace Settings;


void SettingsClientMock::EmitCacheSystemSettingValue(Setting setting,
                                                      const QVariant& value)
{
   emit CacheSystemSettingValue(setting, value);
}

void SettingsClientMock::EmitCacheDisplaySettingValue(Setting setting,
                                                      uint8_t display,
                                                      const QVariant& value)
{
   emit CacheDisplaySettingValue(setting, display, value);
}
