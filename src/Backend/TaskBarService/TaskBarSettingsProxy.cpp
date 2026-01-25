#include "TaskBarSettingsProxy.h"

using namespace Settings;
using namespace TaskBar;

TaskBarSettingsProxy::TaskBarSettingsProxy(QObject* parent)
   : SettingsClient("TaskBarService")
   , Data()
{
   SubscribeToDisplaySettings();
}

Alignment TaskBarSettingsProxy::GetAlignment(const QString& displayID) const
{
   return Data[displayID].Alignment;
}

bool TaskBarSettingsProxy::GetAutoHide(const QString& displayID) const
{
   return Data[displayID].AutoHide;
}

int TaskBarSettingsProxy::GetAutoHideDelayMs(const QString& displayID) const
{
   return Data[displayID].AutoHideDelayMs;
}

int TaskBarSettingsProxy::GetOpacity(const QString& displayID) const
{
   return Data[displayID].Opacity;
}

Orientation TaskBarSettingsProxy::GetOrientation(const QString& displayID) const
{
   return Data[displayID].Orientation;
}

QRect TaskBarSettingsProxy::GetRect(const QString& displayID) const
{
   return Data[displayID].Rect;
}

bool TaskBarSettingsProxy::GetShown(const QString& displayID) const
{
   return Data[displayID].Shown;
}

bool TaskBarSettingsProxy::GetStartButtonShown(const QString& displayID) const
{
   return Data[displayID].StartButtonShown;
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarAlignmentChanged(const QString& displayID,
                                                                       const QVariant& value)
{
   if(value.canConvert<Alignment>())
   {
      const Alignment val = value.value<Alignment>();
      if(val != Data[displayID].Alignment)
      {
         Data[displayID].Alignment = val;
         LogInfo(QString("TaskBar alignment for display %1 changed to %2")
                    .arg(displayID, ToString(val)));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarAutoHideChanged(const QString& displayID,
                                                                      const QVariant& value)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != Data[displayID].AutoHide)
      {
         Data[displayID].AutoHide = val;
         LogInfo(QString("TaskBar AutoHide for display %1 changed to %2")
                    .arg(displayID, val));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarAutoHideDelayMsChanged(
   const QString& displayID,
   const QVariant& value)
{
   if(value.canConvert<int>())
   {
      const bool val = value.toInt();
      if(val != Data[displayID].AutoHideDelayMs)
      {
         Data[displayID].AutoHideDelayMs = val;
         LogInfo(QString("TaskBar auto hide delay (ms) for display %1 changed to %2msec")
                    .arg(displayID, val));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarOpacityChanged(const QString& displayID,
                                                                     const QVariant& value)
{
   if(value.canConvert<int>())
   {
      const bool val = value.toInt();
      if(val != Data[displayID].Opacity)
      {
         Data[displayID].Opacity = val;
         LogInfo(QString("TaskBar opacity for display %1 changed to %2per cent")
                    .arg(displayID, val));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarOrientationChanged(const QString& displayID,
                                                                         const QVariant& value)
{
   if(value.canConvert<Orientation>())
   {
      const Orientation val = value.value<Orientation>();
      if(val != Data[displayID].Orientation)
      {
         Data[displayID].Orientation = val;
         LogInfo(QString("TaskBar orientation for display %1 changed to %2")
                    .arg(displayID, ToString(val)));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarRectChanged(const QString& displayID,
                                                                  const QVariant& value)
{
   if(value.canConvert<QRect>())
   {
      const QRect val = value.value<QRect>();
      if(val != Data[displayID].Rect)
      {
         Data[displayID].Rect = val;
         LogInfo(QString("TaskBar rect for display %1 changed to { %2, %3, %4, %5 }")
                    .arg(displayID,
                         QString::number(val.x()),
                         QString::number(val.y()),
                         QString::number(val.width()),
                         QString::number(val.height())));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarShownChanged(const QString& displayID,
                                                                   const QVariant& value)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != Data[displayID].Shown)
      {
         Data[displayID].Shown = val;
         LogInfo(QString("TaskBar shown for display %1 changed to %2")
                    .arg(displayID, val));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingsTaskBarStartButtonImageChanged(
   const QString& displayID,
   const QVariant& value)
{
   if(value.canConvert<QString>())
   {
      const QString val = value.toString();
      if(val != Data[displayID].StartButtonImagePath)
      {
         Data[displayID].StartButtonImagePath = val;
         LogInfo(QString("TaskBar start button image path for display %1 changed to %2")
                    .arg(displayID, val));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarStartButtonRectChanged(
   const QString& displayID,
   const QVariant& value)
{
   if(value.canConvert<QRect>())
   {
      const QRect val = value.toRect();
      if(val != Data[displayID].StartButtonRect)
      {
         Data[displayID].StartButtonRect = val;
         LogInfo(QString("TaskBar start button rect for display %1 changed to {%2, %3, %4, %5}")
                    .arg(displayID,
                         QString::number(val.x()),
                         QString::number(val.y()),
                         QString::number(val.width()),
                         QString::number(val.height())));

         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::HandleDisplaySettingTaskBarStartButtonShownChanged(
   const QString& displayID,
   const QVariant& value)
{
   if(value.canConvert<bool>())
   {
      const bool val = value.toBool();
      if(val != Data[displayID].StartButtonShown)
      {
         Data[displayID].StartButtonShown = val;
         LogInfo(QString("TaskBar start button shown for display %1 changed to %2")
                    .arg(displayID, val));
         emit SettingsChanged(displayID);
      }
   }
}

void TaskBarSettingsProxy::SubscribeToDisplaySettings()
{
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarAlignment, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarAutoHide, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarAutoHideDelayMs, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarOpacity, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarOrientation, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarRect, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarShown, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarStartButtonImage, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarStartButtonRect, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::TaskBarStartButtonShown, this);
}
