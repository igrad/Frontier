#pragma once

#include "Settings.h"

#include <QObject>

namespace Settings
{
   class SettingsClientInterface: public QObject
   {
      Q_OBJECT

   public:
      ~SettingsClientInterface() = default;

      virtual bool SubscribeToSystemSetting(Setting setting, QObject* subscriber) = 0;
      virtual bool SubscribeToDisplaySetting(Setting setting, QObject* subscriber) = 0;
      virtual bool SubscribeToAllSystemSettings(QObject* subscriber) = 0;
      virtual bool SubscribeToAllDisplaySettings(QObject* subscriber) = 0;
      virtual bool WriteSystemSettingValue(Setting setting, const QVariant& value) = 0;
      virtual bool WriteDisplaySettingValue(Setting setting,
                                            const QString& displayID,
                                            const QVariant& value) = 0;

   signals:
      void CacheSystemSettingValue(Setting setting, const QVariant& value);
      void CacheDisplaySettingValue(Setting setting, const QString& displayID, const QVariant& value);
   };
}
