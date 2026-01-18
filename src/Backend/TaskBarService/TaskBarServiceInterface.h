#pragma once

#include "TaskBarTypes.h"

#include <DisplayInfo.h>

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
      void ViewDataChanged(const DisplayID& displayID, const TaskBar::ViewData& data);

   public slots:
      virtual void HandleDisplayConfigChanged(const DisplayConfigEvent& event) = 0;
   };
}
