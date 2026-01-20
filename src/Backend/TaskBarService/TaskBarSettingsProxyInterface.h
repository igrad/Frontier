#pragma once

#include "TaskBarTypes.h"

namespace TaskBar
{
   class TaskBarSettingsProxyInterface: public QObject
   {
      Q_OBJECT

   public:
      ~TaskBarSettingsProxyInterface() = default;

      virtual Alignment GetAlignment(const QString& displayID) const = 0;
      virtual bool GetAutoHide(const QString& displayID) const = 0;
      virtual int GetAutoHideDelayMs(const QString& displayID) const = 0;
      virtual int GetOpacity(const QString& displayID) const = 0;
      virtual Orientation GetOrientation(const QString& displayID) const = 0;
      virtual QRect GetRect(const QString& displayID) const = 0;
      virtual bool GetShown(const QString& displayID) const = 0;
      virtual bool GetStartButtonShown(const QString& displayID) const = 0;

   signals:
      void SettingsChanged(const QString& id);

   public slots:
      virtual void HandleDisplaySettingTaskBarAlignmentChanged(const QString& displayID,
                                                               const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarAutoHideChanged(const QString& displayID,
                                                              const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarAutoHideDelayMsChanged(const QString& displayID,
                                                                     const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarOpacityChanged(const QString& displayID,
                                                             const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarOrientationChanged(const QString& displayID,
                                                                 const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarRectChanged(const QString& displayID,
                                                          const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarShownChanged(const QString& displayID,
                                                           const QVariant& value) = 0;
      virtual void HandleDisplaySettingTaskBarStartButtonShownChanged(const QString& displayID,
                                                                      const QVariant& value) = 0;
   };
}
