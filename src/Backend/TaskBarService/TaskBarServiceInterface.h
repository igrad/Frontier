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

   signals:
      void ViewDataChanged(const TaskBar::ViewData& data);
   };
}
