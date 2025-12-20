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

      virtual bool SubscribeToSetting(Setting setting, QObject* subscriber) = 0;
      virtual bool WriteSettingValue(Setting setting, const QVariant& value) = 0;

   signals:
      void CacheSettingValue(Setting setting, const QVariant& value);
   };
}
