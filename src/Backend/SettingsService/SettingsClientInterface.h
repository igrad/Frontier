#pragma once

#include "Settings.h"

#include <QObject>

namespace Settings
{
   typedef void(*SettingUpdateSlot)(const Setting& setting, const QVariant& value);

   class SettingsClientInterface: public QObject
   {
      Q_OBJECT

   public:
      ~SettingsClientInterface() = default;

      virtual void SubscribeToSetting(const Setting& setting, SettingUpdateSlot) = 0;
   };
}
