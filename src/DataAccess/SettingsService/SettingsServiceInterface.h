#pragma once

#include "Settings.h"

#include <QObject>
#include <QVariant>

namespace Settings
{
   class SettingsServiceInterface: public QObject
   {
   Q_OBJECT

   public:
      ~SettingsServiceInterface() = default;

      virtual void FetchAllSettings() = 0;

   public slots:
      virtual void HandleCacheSettingValue(const Setting setting,
                                           const QVariant value) = 0;

   signals:
      void SettingUpdated(const Setting setting, const QVariant val);
   };
}
