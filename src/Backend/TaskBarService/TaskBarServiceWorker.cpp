#include "TaskBarServiceWorker.h"

using namespace TaskBar;

TaskBarServiceWorker::TaskBarServiceWorker(const DisplayInfo& info,
                                           TaskBarSettingsProxyInterface* settingsProxy,
                                           QObject* parent)
   : QObject(parent)
   , DisplayID(info.ID)
   , Info(info)
   , SettingsProxy(settingsProxy)
{

}
TaskBarServiceWorker::TaskBarServiceWorker(const TaskBarServiceWorker& other)
   : QObject(other.parent())
   , DisplayID(other.DisplayID)
   , Info(other.Info)
   , SettingsProxy(other.SettingsProxy)
{

}

QString TaskBarServiceWorker::GetDisplayID() const
{
   return DisplayID;
}

DisplayInfo TaskBarServiceWorker::GetDisplayInfo() const
{
   return Info;
}

// bool TaskBarServiceWorker::operator==(const TaskBarServiceWorker& rhs) const
// {
//    return Info == rhs.Info;
// }

// TaskBarServiceWorker& TaskBarServiceWorker::operator=(const TaskBarServiceWorker& rhs)
// {
//    DisplayID = rhs.DisplayID;
//    Info = rhs.Info;

// }

void TaskBarServiceWorker::HandleSettingsChanged()
{
   ViewData data;
   data.Alignment = SettingsProxy->GetAlignment(DisplayID);
   data.DisplayID = DisplayID;
   data.AutoHide = SettingsProxy->GetAutoHide(DisplayID);
   data.AutoHideDelayMs = SettingsProxy->GetAutoHideDelayMs(DisplayID);
   data.Opacity = SettingsProxy->GetOpacity(DisplayID);
   data.Direction = SettingsProxy->GetDirection(DisplayID);

   emit TaskBarViewDataChanged(DisplayID, data);
}

void TaskBarServiceWorker::HandleDisplayConfigChanged(const DisplayInfo& info)
{
   // TODO
}

void TaskBarServiceWorker::HandleDisplayRemoved(const DisplayInfo& info)
{
   // TODO
   // Probably have to notify the view somehow?
}
