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

      QString GetDisplayID() const;
      DisplayInfo GetDisplayInfo() const;

      // bool operator==(const TaskBarServiceWorker& rhs) const;
      // TaskBarServiceWorker& operator=(const TaskBarServiceWorker& rhs);

   public slots:
      void HandleSettingsChanged();
      void HandleDisplayConfigChanged(const DisplayInfo& info);
      void HandleDisplayRemoved(const DisplayInfo& info);

   signals:
      void TaskBarViewDataChanged(const QString& displayID,
                                  const TaskBar::ViewData& data);

   private:
      QString DisplayID;
      DisplayInfo Info;
      TaskBarSettingsProxy* SettingsProxy;
   };
}
