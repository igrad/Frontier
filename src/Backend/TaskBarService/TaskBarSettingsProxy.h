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
      void HandleSettingTaskBarAlignmentChanged(const QVariant& value);
      void HandleSettingTaskBarAutoHideChanged(const QVariant& value);
      void HandleSettingTaskBarHideDurationChanged(const QVariant& value);
      void HandleSettingTaskBarOpacityChanged(const QVariant& value);
      void HandleSettingTaskBarOrientationChanged(const QVariant& value);
      void HandleSettingTaskBarShownChanged(const QVariant& value);

   private:
      void SubscribeToSettings();

      Settings::SettingsClient SettingsClient;

      Alignment CurrentAlignment;
      bool CurrentAutoHide;
      int CurrentHideDurationMsec;
      int CurrentOpacity;
      Orientation CurrentOrientation;
      bool CurrentShown;
   };
}
