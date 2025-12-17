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

      virtual void SubscribeToSetting(const Setting& setting, QObject* subscriber) = 0;
   };
}
