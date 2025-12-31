#include "TaskBarService.h"

#include <SettingsService/Settings.h>

using namespace TaskBar;

TaskBarService::TaskBarService(QObject* parent)
   : SettingsClient("TaskBarService")
   , CurrentAlignment(Alignment::Bottom)
   , CurrentOrientation(Orientation::Left)
{
   setParent(parent);

   SubscribeToSettings();
}

void TaskBarService::RegisterMetaTypes() const
{
   qRegisterMetaType<TaskBar::ViewData>("TaskBar::ViewData");
}

void TaskBarService::HandleSettingTaskBarAlignmentChanged(const QVariant& value)
{
   if(value.canConvert<Alignment>())
   {
      const Alignment val = value.value<Alignment>();

      if(val != CurrentAlignment)
      {
         CurrentAlignment = val;
         emit TaskBarAlignmentChanged(val);
      }
   }
}

void TaskBarService::HandleSettingTaskBarOrientationChanged(const QVariant& value)
{
   if(value.canConvert<Orientation>())
   {
      const Orientation val = value.value<Orientation>();

      if(val != CurrentOrientation)
      {
         CurrentOrientation = val;
         emit TaskBarOrientationChanged(val);
      }
   }
}

void TaskBarService::SubscribeToSettings()
{
   SettingsClient.SubscribeToSetting(Settings::Setting::TaskBarAlignment, this);
   SettingsClient.SubscribeToSetting(Settings::Setting::TaskBarOrientation, this);
}
