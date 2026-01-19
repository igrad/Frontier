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

      Alignment GetAlignment(const QString& displayID) const;
      bool GetAutoHide(const QString& displayID) const;
      int GetAutoHideDelayMs(const QString& displayID) const;
      int GetOpacity(const QString& displayID) const;
      Orientation GetOrientation(const QString& displayID) const;
      QRect GetRect(const QString& displayID) const;
      bool GetShown(const QString& displayID) const;
      bool GetStartButtonShown(const QString& displayID) const;

   signals:
      void SettingsChanged(QString id);

   public slots:
      void HandleDisplaySettingTaskBarAlignmentChanged(const QString& displayID,
                                                       const QVariant& value);
      void HandleDisplaySettingTaskBarAutoHideChanged(const QString& displayID,
                                                      const QVariant& value);
      void HandleDisplaySettingTaskBarAutoHideDelayMsChanged(const QString& displayID,
                                                             const QVariant& value);
      void HandleDisplaySettingTaskBarOpacityChanged(const QString& displayID,
                                                     const QVariant& value);
      void HandleDisplaySettingTaskBarOrientationChanged(const QString& displayID,
                                                         const QVariant& value);
      void HandleDisplaySettingTaskBarRectChanged(const QString& displayID,
                                                  const QVariant& value);
      void HandleDisplaySettingTaskBarShownChanged(const QString& displayID,
                                                   const QVariant& value);
      void HandleDisplaySettingTaskBarStartButtonShownChanged(const QString& displayID,
                                                              const QVariant& value);

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QMap<QString, ViewData> Data;
   };
}
