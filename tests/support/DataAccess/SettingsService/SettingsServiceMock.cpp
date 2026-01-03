#include "SettingsServiceMock.h"

void SettingsServiceMock::EmitSettingUpdated(const Settings::Setting setting,
                                             const QVariant& val)
{
   emit SettingUpdated(setting, val);
}
