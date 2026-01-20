#pragma once

#include "TaskBarServiceInterface.h"
#include "TaskBarServiceWorker.h"
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

   public slots:
      void HandleDisplayConfigChanged(const DisplayConfigEvent& event) override;

   private slots:
      void HandleSettingsChanged(const DisplayID& id);

   private:
      TaskBarSettingsProxy SettingsProxy;
      QMap<DisplayID, TaskBarServiceWorker*> Workers;
   };
}
