#pragma once

#include "TaskBarTypes.h"

#include <Settings.h>
#include <SettingsClient.h>

#include <QRect>

namespace TaskBar
{
   class TaskBarSettingsProxy: public QObject
   {
      Q_OBJECT

   public:
      TaskBarSettingsProxy(QObject* parent = nullptr);
      ~TaskBarSettingsProxy() = default;

      const Alignment GetAlignment(uint8_t display) const;
      const bool GetAutoHide(uint8_t display) const;
      const int GetAutoHideDelayMs(uint8_t display) const;
      const int GetOpacity(uint8_t display) const;
      const Orientation GetOrientation(uint8_t display) const;
      const QRect GetRect(uint8_t display) const;
      const bool GetShown(uint8_t display) const;
      const bool GetStartButtonShown(uint8_t display) const;

   signals:
      void SettingsChanged(uint8_t display);

   public slots:
      void HandleDisplaySettingTaskBarAlignmentChanged(const QVariant& value,
                                                       uint8_t displayNum);
      void HandleDisplaySettingTaskBarAutoHideChanged(const QVariant& value,
                                                      uint8_t displayNum);
      void HandleDisplaySettingTaskBarAutoHideDelayMsChanged(const QVariant& value,
                                                          uint8_t displayNum);
      void HandleDisplaySettingTaskBarOpacityChanged(const QVariant& value,
                                                     uint8_t displayNum);
      void HandleDisplaySettingTaskBarOrientationChanged(const QVariant& value,
                                                         uint8_t displayNum);
      void HandleDisplaySettingTaskBarRectChanged(const QVariant& value,
                                                  uint8_t displayNum);
      void HandleDisplaySettingTaskBarShownChanged(const QVariant& value,
                                                   uint8_t displayNum);
      void HandleDisplaySettingTaskBarStartButtonShownChanged(const QVariant& value,
                                                              uint8_t displayNum);

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QList<ViewData> Data;
   };
}
