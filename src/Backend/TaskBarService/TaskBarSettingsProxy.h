#pragma once

#include "TaskBarSettingsProxyInterface.h"

#include <Settings.h>
#include <SettingsClient.h>

#include <QRect>

namespace TaskBar
{
   class TaskBarSettingsProxy: public TaskBarSettingsProxyInterface
   {
      Q_OBJECT

   public:
      explicit TaskBarSettingsProxy(QObject* parent = nullptr);
      ~TaskBarSettingsProxy() = default;

      Alignment GetAlignment(const QString& displayID) const override;
      bool GetAutoHide(const QString& displayID) const override;
      int GetAutoHideDelayMs(const QString& displayID) const override;
      int GetOpacity(const QString& displayID) const override;
      Orientation GetOrientation(const QString& displayID) const override;
      QRect GetRect(const QString& displayID) const override;
      bool GetShown(const QString& displayID) const override;
      bool GetStartButtonShown(const QString& displayID) const override;

   public slots:
      void HandleDisplaySettingTaskBarAlignmentChanged(const QString& displayID,
                                                       const QVariant& value) override;
      void HandleDisplaySettingTaskBarAutoHideChanged(const QString& displayID,
                                                      const QVariant& value) override;
      void HandleDisplaySettingTaskBarAutoHideDelayMsChanged(const QString& displayID,
                                                             const QVariant& value) override;
      void HandleDisplaySettingTaskBarOpacityChanged(const QString& displayID,
                                                     const QVariant& value) override;
      void HandleDisplaySettingTaskBarOrientationChanged(const QString& displayID,
                                                         const QVariant& value) override;
      void HandleDisplaySettingTaskBarRectChanged(const QString& displayID,
                                                  const QVariant& value) override;
      void HandleDisplaySettingTaskBarShownChanged(const QString& displayID,
                                                   const QVariant& value) override;
      void HandleDisplaySettingsTaskBarStartButtonImageChanged(const QString& displayID,
                                                               const QVariant& value) override;
      void HandleDisplaySettingTaskBarStartButtonRectChanged(const QString& displayID,
                                                             const QVariant& value) override;
      void HandleDisplaySettingTaskBarStartButtonShownChanged(const QString& displayID,
                                                              const QVariant& value) override;

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QMap<QString, ViewData> Data;
   };
}
