#pragma once

#include "TaskBarSettingsProxy.h"

#include <DisplayInfo.h>

namespace TaskBar
{
   class TaskBarServiceWorker: public QObject
   {
      Q_OBJECT

   public:
      TaskBarServiceWorker(const DisplayInfo& info,
                           TaskBarSettingsProxy* settingsProxy,
                           QObject* parent = nullptr);
      TaskBarServiceWorker(const TaskBarServiceWorker& other);
      ~TaskBarServiceWorker() = default;

      uint8_t GetDisplayNum() const;
      DisplayInfo GetDisplayInfo() const;

      // bool operator==(const TaskBarServiceWorker& rhs) const;
      // TaskBarServiceWorker& operator=(const TaskBarServiceWorker& rhs);

   public slots:
      void HandleSettingsChanged();

   signals:
      void TaskBarViewDataChanged(uint8_t displayNum,
                                  const TaskBar::ViewData& data);

   private:
      uint8_t DisplayNum;
      DisplayInfo Info;
      TaskBarSettingsProxy* SettingsProxy;
   };
}
