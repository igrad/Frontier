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
      void HandleDisplaySettingTaskBarAlignmentChanged(const QVariant& value,
                                                       const QString& displayID);
      void HandleDisplaySettingTaskBarAutoHideChanged(const QVariant& value,
                                                      const QString& displayID);
      void HandleDisplaySettingTaskBarAutoHideDelayMsChanged(const QVariant& value,
                                                             const QString& displayID);
      void HandleDisplaySettingTaskBarOpacityChanged(const QVariant& value,
                                                     const QString& displayID);
      void HandleDisplaySettingTaskBarOrientationChanged(const QVariant& value,
                                                         const QString& displayID);
      void HandleDisplaySettingTaskBarRectChanged(const QVariant& value,
                                                  const QString& displayID);
      void HandleDisplaySettingTaskBarShownChanged(const QVariant& value,
                                                   const QString& displayID);
      void HandleDisplaySettingTaskBarStartButtonShownChanged(const QVariant& value,
                                                              const QString& displayID);

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QMap<QString, ViewData> Data;
   };
}
