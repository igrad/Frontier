#include "SettingsServiceMock.h"

void SettingsServiceMock::EmitSystemSettingUpdated(const Settings::Setting setting,
                                                   const QVariant& val)
{
   emit SystemSettingUpdated(setting, val);
}

void SettingsServiceMock::EmitDisplaySettingUpdated(const Settings::Setting setting,
                                                    const QString& displayID,
                                                    const QVariant& val)
{
   emit DisplaySettingUpdated(setting, displayID, val);
}
