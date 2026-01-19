#include "TaskBarService.h"

using namespace TaskBar;

TaskBarService::TaskBarService(QObject* parent)
   : SettingsProxy(this)
   , Workers()
{
   setParent(parent);

   connect(&SettingsProxy, &TaskBarSettingsProxy::SettingsChanged,
           this, &TaskBarService::HandleSettingsChanged);
}

void TaskBarService::RegisterMetaTypes() const
{
   qRegisterMetaType<TaskBar::ViewData>("TaskBar::ViewData");
}

void TaskBarService::HandleDisplayConfigChanged(const DisplayConfigEvent& event)
{
   // If there are no current workers, just add them quickly. This is at startup.
   if(Workers.isEmpty())
   {
      for(const QPair<DisplayConfigEventType, DisplayInfo>& info : std::as_const(event.Displays))
      {
         Workers[info.second.ID] = new TaskBarServiceWorker(info.second,
                                                            &SettingsProxy,
                                                            this);
      }
   }
   else
   {
      for(const QPair<DisplayConfigEventType, DisplayInfo>& info : std::as_const(event.Displays))
      {
         switch(info.first)
         {
         case DisplayConfigEventType::Added:
            Workers[info.second.ID] = new TaskBarServiceWorker(info.second,
                                                               &SettingsProxy,
                                                               this);
            break;
         case DisplayConfigEventType::Changed:
            Workers[info.second.ID]->HandleDisplayConfigChanged(info.second);
            break;
         case DisplayConfigEventType::Removed:
            Workers[info.second.ID]->HandleDisplayRemoved(info.second);
            break;
         default:
            LogWarn(QString("Received None event for display ID: %1")
                       .arg(info.second.ID));
            break;
         }
      }
   }
}

void TaskBarService::HandleSettingsChanged(const DisplayID& id)
{
   Workers[id]->HandleSettingsChanged();
}
