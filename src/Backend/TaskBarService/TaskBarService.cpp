#include "TaskBarService.h"

using namespace TaskBar;

TaskBarService::TaskBarService(QObject* parent)
   : SettingsProxy(this)
{
   setParent(parent);
}

void TaskBarService::RegisterMetaTypes() const
{
   qRegisterMetaType<TaskBar::ViewData>("TaskBar::ViewData");
}

void TaskBarService::HandleSettingsChanged()
{
   ViewData data;
   data.Alignment = SettingsProxy.GetAlignment();
   data.AssignedMonitor = 0; // TODO: Multiple monitors
   data.AutoHide = SettingsProxy.GetAutoHide();
   data.AutoHideDelayMs = SettingsProxy.GetHideDuration();
   data.Opacity = SettingsProxy.GetOpacity();
   data.Orientation = SettingsProxy.GetOrientation();

   emit ViewDataChanged(data);
}
