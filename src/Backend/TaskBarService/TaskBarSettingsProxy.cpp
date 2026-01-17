#include "TaskBarSettingsProxy.h"

using namespace Settings;
using namespace TaskBar;

TaskBarSettingsProxy::TaskBarSettingsProxy(QObject* parent)
   : SettingsClient("TaskBarService")
   , CurrentAlignment(Alignment::Bottom)
   , CurrentAutoHide(false)
   , CurrentHideDurationMsec(0)
   , CurrentOrientation(Orientation::LeftToRight)
   , CurrentShown(true)
{
   SubscribeToSystemSettings();
}

const Alignment TaskBarSettingsProxy::GetAlignment() const
{
   return CurrentAlignment;
}

const bool TaskBarSettingsProxy::GetAutoHide() const
{
   return CurrentAutoHide;
}

const int TaskBarSettingsProxy::GetHideDuration() const
{
   return CurrentHideDurationMsec;
}

const int TaskBarSettingsProxy::GetOpacity() const
{
   return CurrentOpacity;
}

const Orientation TaskBarSettingsProxy::GetOrientation() const
{
   return CurrentOrientation;
}

const bool TaskBarSettingsProxy::GetShown() const
{
   return CurrentShown;
}

void TaskBarSettingsProxy::HandleSettingTaskBarAlignmentChanged(const QVariant& value)
{
   if(value.canConvert<Alignment>())
   {
      const Alignment val = value.value<Alignment>();
      if(val != CurrentAlignment)
      {
         CurrentAlignment = val;
         LogInfo(QString("TaskBar alignment changed to %1").arg(ToString(val)));

         emit SettingsChanged();
      }
   }
}

void TaskBarSettingsProxy::HandleSettingTaskBarAutoHideChanged(const QVariant& value)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != CurrentAutoHide)
      {
         CurrentAutoHide = val;
         LogInfo(QString("TaskBar AutoHide changed to %1").arg(val));

         emit SettingsChanged();
      }
   }
}

void TaskBarSettingsProxy::HandleSettingTaskBarHideDurationChanged(const QVariant& value)
{
   if(value.canConvert<int>())
   {
      const bool val = value.toInt();
      if(val != CurrentHideDurationMsec)
      {
         CurrentHideDurationMsec = val;
         LogInfo(QString("TaskBar Hide Duration changed to %1msec").arg(val));

         emit SettingsChanged();
      }
   }
}

void TaskBarSettingsProxy::HandleSettingTaskBarOpacityChanged(const QVariant& value)
{
   if(value.canConvert<int>())
   {
      const bool val = value.toInt();
      if(val != CurrentOpacity)
      {
         CurrentOpacity = val;
         LogInfo(QString("TaskBar opacity changed to %1per cent").arg(val));

         emit SettingsChanged();
      }
   }
}

void TaskBarSettingsProxy::HandleSettingTaskBarOrientationChanged(const QVariant& value)
{
   if(value.canConvert<Orientation>())
   {
      const Orientation val = value.value<Orientation>();
      if(val != CurrentOrientation)
      {
         CurrentOrientation = val;
         LogInfo(QString("TaskBar orientation changed to %1").arg(ToString(val)));

         emit SettingsChanged();
      }
   }
}

void TaskBarSettingsProxy::HandleSettingTaskBarShownChanged(const QVariant& value)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != CurrentShown)
      {
         CurrentShown = val;
         LogInfo(QString("TaskBar shown changed to %1").arg(val));

         emit SettingsChanged();
      }
   }
}

void TaskBarSettingsProxy::SubscribeToSystemSettings()
{
   SettingsClient.SubscribeToSystemSetting(Setting::TaskBarAlignment, this);
   SettingsClient.SubscribeToSystemSetting(Setting::TaskBarAutoHide, this);
   SettingsClient.SubscribeToSystemSetting(Setting::TaskBarHideDuration, this);
   SettingsClient.SubscribeToSystemSetting(Setting::TaskBarOrientation, this);
   SettingsClient.SubscribeToSystemSetting(Setting::TaskBarShown, this);
}
