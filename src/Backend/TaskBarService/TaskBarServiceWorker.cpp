#include "TaskBarServiceWorker.h"

using namespace TaskBar;

TaskBarServiceWorker::TaskBarServiceWorker(const DisplayInfo& info,
                                           TaskBarSettingsProxy* settingsProxy,
                                           QObject* parent)
   : QObject(parent)
   , DisplayNum(info.Number)
   , Info(info)
   , SettingsProxy(settingsProxy)
{

}
TaskBarServiceWorker::TaskBarServiceWorker(const TaskBarServiceWorker& other)
   : QObject(other.parent())
   , DisplayNum(other.DisplayNum)
   , Info(other.Info)
   , SettingsProxy(other.SettingsProxy)
{

}

uint8_t TaskBarServiceWorker::GetDisplayNum() const
{
   return DisplayNum;
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
//    DisplayNum = rhs.DisplayNum;
//    Info = rhs.Info;

// }

void TaskBarServiceWorker::HandleSettingsChanged()
{
   ViewData data;
   data.Alignment = SettingsProxy->GetAlignment(DisplayNum);
   data.AssignedMonitor = DisplayNum;
   data.AutoHide = SettingsProxy->GetAutoHide(DisplayNum);
   data.AutoHideDelayMs = SettingsProxy->GetAutoHideDelayMs(DisplayNum);
   data.Opacity = SettingsProxy->GetOpacity(DisplayNum);
   data.Orientation = SettingsProxy->GetOrientation(DisplayNum);

   emit TaskBarViewDataChanged(DisplayNum, data);
}
