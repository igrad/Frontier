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
      void ViewDataChanged(uint8_t display, const TaskBar::ViewData& data);

   public slots:
      virtual void HandleDisplayConfigChanged(const DisplayEvent& event,
                                              const QMap<QString, DisplayInfo>& displays) = 0;
   };
}
