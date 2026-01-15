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
      virtual void HandleCacheSystemSettingValue(const Setting setting,
                                                 const QVariant value) = 0;
      virtual void HandleCacheDisplaySettingValue(const Setting setting,
                                                  uint8_t display,
                                                  const QVariant value) = 0;
      virtual void HandleRequestSystemSettingValue(const Setting setting) = 0;
      virtual void HandleRequestDisplaySettingValue(const Setting setting,
                                                    uint8_t display) = 0;

   signals:
      void SystemSettingUpdated(const Setting setting, const QVariant val);
      void DisplaySettingUpdated(const Setting setting, uint8_t display, const QVariant val);
   };
}
