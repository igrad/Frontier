#pragma once

#include "TaskBarTypes.h"

#include <Settings.h>
#include <SettingsClient.h>

namespace TaskBar
{
   class TaskBarSettingsProxy: public QObject
   {
      Q_OBJECT

   public:
      TaskBarSettingsProxy(QObject* parent = nullptr);
      ~TaskBarSettingsProxy() = default;

      const Alignment GetAlignment() const;
      const bool GetAutoHide() const;
      const int GetHideDuration() const;
      const int GetOpacity() const;
      const Orientation GetOrientation() const;
      const bool GetShown() const;

   signals:
      void SettingsChanged();

   public slots:
      void HandleDisplaySettingTaskBarAlignmentChanged(const QVariant& value,
                                                       uint8_t displayNum);
      void HandleDisplaySettingTaskBarAutoHideChanged(const QVariant& value,
                                                      uint8_t displayNum);
      void HandleDisplaySettingTaskBarHideDurationChanged(const QVariant& value,
                                                          uint8_t displayNum);
      void HandleDisplaySettingTaskBarOpacityChanged(const QVariant& value,
                                                     uint8_t displayNum);
      void HandleDisplaySettingTaskBarOrientationChanged(const QVariant& value,
                                                         uint8_t displayNum);
      void HandleDisplaySettingTaskBarShownChanged(const QVariant& value,
                                                   uint8_t displayNum);

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;

      Alignment CurrentAlignment;
      bool CurrentAutoHide;
      int CurrentHideDurationMsec;
      int CurrentOpacity;
      Orientation CurrentOrientation;
      bool CurrentShown;
   };
}
