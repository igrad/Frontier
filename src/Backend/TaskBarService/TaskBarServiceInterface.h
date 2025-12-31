#pragma once

#include "TaskBarTypes.h"

#include <QObject>

namespace TaskBar
{
   class TaskBarServiceInterface: public QObject
   {
      Q_OBJECT

   public:
      ~TaskBarServiceInterface() = default;

      virtual void RegisterMetaTypes() const = 0;

   public slots:
      virtual void HandleSettingTaskBarAlignmentChanged(const QVariant& value) = 0;
      virtual void HandleSettingTaskBarOrientationChanged(const QVariant& value) = 0;

   signals:
      void ViewDataChanged(const TaskBar::ViewData& data);
   };
}
