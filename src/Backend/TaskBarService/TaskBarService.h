#pragma once

#include "TaskBarServiceInterface.h"
#include "TaskBarSettingsProxy.h"

#include <QTimer>

namespace TaskBar
{
   class TaskBarService: public TaskBarServiceInterface
   {
      Q_OBJECT

   public:
      explicit TaskBarService(QObject* parent = nullptr);
      ~TaskBarService() = default;

      void RegisterMetaTypes() const override;

   private slots:
      void HandleSettingsChanged();

   private:
      TaskBarSettingsProxy SettingsProxy;
      QTimer AutoHideTimer;
   };
}
