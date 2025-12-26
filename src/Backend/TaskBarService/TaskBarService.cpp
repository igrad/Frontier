#include "TaskBarService.h"

#include <Settings.h>

using namespace TaskBar;

TaskBarService::TaskBarService(QObject* parent)
   : SettingsClient("TaskBarService")
   , CurrentAlignment(Alignment::Bottom)
   , CurrentDirectionality(Directionality::Left)
{
   setParent(parent);

   SubscribeToSettings();
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

void TaskBarService::HandleSettingTaskBarDirectionalityChanged(const QVariant& value)
{
   if(value.canConvert<Directionality>())
   {
      const Directionality val = value.value<Directionality>();

      if(val != CurrentDirectionality)
      {
         CurrentDirectionality = val;
         emit TaskBarDirectionalityChanged(val);
      }
   }
}

void TaskBarService::SubscribeToSettings()
{
   SettingsClient.SubscribeToSetting(Settings::Setting::TaskBarAlignment, this);
   SettingsClient.SubscribeToSetting(Settings::Setting::TaskBarDirectionality, this);
}
