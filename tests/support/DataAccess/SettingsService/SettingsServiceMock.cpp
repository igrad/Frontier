#include "SettingsServiceMock.h"

void SettingsServiceMock::EmitSystemSettingUpdated(const Settings::Setting setting,
                                                   const QVariant& val)
{
   emit SystemSettingUpdated(setting, val);
}

void SettingsServiceMock::EmitDisplaySettingUpdated(const Settings::Setting setting,
                                                    uint8_t display,
                                                    const QVariant& val)
{
   emit DisplaySettingUpdated(setting, display, val);
}
