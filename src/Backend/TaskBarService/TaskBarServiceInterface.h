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

   public slots:
      virtual void HandleSettingTaskBarAlignmentChanged(const QVariant& value) = 0;
      virtual void HandleSettingTaskBarDirectionalityChanged(const QVariant& value) = 0;

   signals:
      void TaskBarAlignmentChanged(Alignment value);
      void TaskBarDirectionalityChanged(Directionality value);
   };
}
