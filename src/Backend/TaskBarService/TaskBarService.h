#pragma once

#include "TaskBarServiceInterface.h"
#include "TaskBarServiceWorker.h"

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
      void HandleDisplayConfigChanged(const DisplayEvent& event,
                                      const QMap<QString, DisplayInfo>& displays) override;

   private slots:
      void HandleSettingsChanged(uint8_t displayNum);

   private:
      TaskBarSettingsProxy SettingsProxy;
      QMap<uint8_t, TaskBarServiceWorker*> Workers;
   };
}
