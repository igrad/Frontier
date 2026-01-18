#include "TaskBarSettingsProxy.h"

using namespace Settings;
using namespace TaskBar;

TaskBarSettingsProxy::TaskBarSettingsProxy(QObject* parent)
   : SettingsClient("TaskBarService")
   , Data()
{
   SubscribeToDisplaySettings();
}

const Alignment TaskBarSettingsProxy::GetAlignment(uint8_t display) const
{
   return Data[display].Alignment;
}

const bool TaskBarSettingsProxy::GetAutoHide(uint8_t display) const
{
   return Data[display].AutoHide;
}

const int TaskBarSettingsProxy::GetAutoHideDelayMs(uint8_t display) const
{
   return Data[display].AutoHideDelayMs;
}

const int TaskBarSettingsProxy::GetOpacity(uint8_t display) const
{
   return Data[display].Opacity;
}

const Orientation TaskBarSettingsProxy::GetOrientation(uint8_t display) const
{
   return Data[display].Orientation;
}

const QRect TaskBarSettingsProxy::GetRect(uint8_t display) const
{
   return Data[display].Rect;
}

const bool TaskBarSettingsProxy::GetShown(uint8_t display) const
{
   return Data[display].Shown;
}

const bool TaskBarSettingsProxy::GetStartButtonShown(uint8_t display) const
{
   return Data[display].StartButtonShown;
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarAlignmentChanged(const QVariant& value,
                                                                       uint8_t displayNum)
{
   if(value.canConvert<Alignment>())
   {
      const Alignment val = value.value<Alignment>();
      if(val != Data[displayNum].Alignment)
      {
         Data[displayNum].Alignment = val;
         LogInfo(QString("TaskBar alignment for display %1 changed to %2")
                    .arg(QString::number(displayNum), ToString(val)));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarAutoHideChanged(const QVariant& value,
                                                                      uint8_t displayNum)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != Data[displayNum].AutoHide)
      {
         Data[displayNum].AutoHide = val;
         LogInfo(QString("TaskBar AutoHide for display %1 changed to %2")
                    .arg(QString::number(displayNum), val));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarAutoHideDelayMsChanged(const QVariant& value,
                                                                             uint8_t displayNum)
{
   if(value.canConvert<int>())
   {
      const bool val = value.toInt();
      if(val != Data[displayNum].AutoHideDelayMs)
      {
         Data[displayNum].AutoHideDelayMs = val;
         LogInfo(QString("TaskBar auto hide delay (ms) for display %1 changed to %2msec")
                    .arg(QString::number(displayNum), val));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarOpacityChanged(const QVariant& value,
                                                                     uint8_t displayNum)
{
   if(value.canConvert<int>())
   {
      const bool val = value.toInt();
      if(val != Data[displayNum].Opacity)
      {
         Data[displayNum].Opacity = val;
         LogInfo(QString("TaskBar opacity for display %1 changed to %2per cent")
                    .arg(QString::number(displayNum), val));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarOrientationChanged(const QVariant& value,
                                                                         uint8_t displayNum)
{
   if(value.canConvert<Orientation>())
   {
      const Orientation val = value.value<Orientation>();
      if(val != Data[displayNum].Orientation)
      {
         Data[displayNum].Orientation = val;
         LogInfo(QString("TaskBar orientation for display %1 changed to %2")
                    .arg(QString::number(displayNum), ToString(val)));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarRectChanged(const QVariant& value,
                                                  uint8_t displayNum)
{
   if(value.canConvert<QRect>())
   {
      const QRect val = value.value<QRect>();
      if(val != Data[displayNum].Rect)
      {
         Data[displayNum].Rect = val;
         LogInfo(QString("TaskBar rect for display %1 changed to { %2, %3, %4, %5 }")
                    .arg(QString::number(displayNum),
                         QString::number(val.x()),
                         QString::number(val.y()),
                         QString::number(val.width()),
                         QString::number(val.height())));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarShownChanged(const QVariant& value,
                                                                   uint8_t displayNum)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != Data[displayNum].Shown)
      {
         Data[displayNum].Shown = val;
         LogInfo(QString("TaskBar shown for display %1 changed to %2")
                    .arg(QString::number(displayNum), val));

         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarStartButtonShownChanged(const QVariant& value,
                                                                              uint8_t displayNum)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != Data[displayNum].StartButtonShown)
      {
         Data[displayNum].StartButtonShown = val;
         LogInfo(QString("TaskBar start button shown for display %1 changed to %2")
                    .arg(QString::number(displayNum), val));
         emit SettingsChanged(displayNum);
      }
   }
}

void TaskBarSettingsProxy::SubscribeToDisplaySettings()
{
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarAlignment, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarAutoHide, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarHideDuration, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarOrientation, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarShown, this);
}
