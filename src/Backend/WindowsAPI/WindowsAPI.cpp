#include "WindowsAPI.h"
#include "WindowsEventMessageFilter.h"

#include <DisplayInfo.h>

#include <Log.h>

#include <windows.h>
#include <shellscalingapi.h>

// NOTE: For now, the WindowsAPI class directly interfaces with windows.h. This isn't very
// testable, but it's fine for now. We will eventually need to make a 1:1 wrapper for the windows
// API so that we can mock it and test this thoroughly.

WindowsAPI::WindowsAPI(const WindowsEventMessageFilter& filter,
                       QObject* parent)
   : CachedSettings()
{
   setParent(parent);

   ConnectToEventMessageFilter(filter);
}

QVariant WindowsAPI::GetCurrentSettingValue(Windows::Setting setting)
{
   QVariant retVal;

   const auto cachedSettingsIter = CachedSettings.constFind(setting);

   if(cachedSettingsIter != CachedSettings.cend())
   {
      retVal = *cachedSettingsIter;
   }
   else
   {
      switch(setting)
      {
      case Windows::Setting::NumberOfDetectedMonitors:
         retVal = GetSystemMetrics(SM_CMONITORS);
         break;
      default:
         LogWarn(QString("Unhandled setting: \"%1\"").arg(ToString(setting)));
         break;
      }

      CachedSettings[setting] = retVal;
   }

   return retVal;
}

void WindowsAPI::HandlePollDisplaysInfo()
{
   GetAllDisplayInfo();
}

void WindowsAPI::HandleWindowsSettingUpdated(const Windows::Setting setting,
                                      const QVariant& value)
{
   Q_UNUSED(setting)
   Q_UNUSED(value)
}

void WindowsAPI::HandleDevicesChanged()
{
   GetAllDisplayInfo();
}

BOOL CALLBACK WindowsAPI::MonitorEnumProc(HMONITOR hMonitor,
                                 HDC hdcMonitor,
                                 LPRECT lprcMonitor,
                                 LPARAM dwData)
{
   WindowsAPI* me = reinterpret_cast<WindowsAPI*>(dwData);
   me->MonitorDatumReceived(hMonitor, hdcMonitor, lprcMonitor);

   return TRUE;
}

void WindowsAPI::ConnectToEventMessageFilter(const WindowsEventMessageFilter& filter)
{
   connect(&filter, &WindowsEventMessageFilter::WindowsSettingUpdated,
           this, &WindowsAPI::HandleWindowsSettingUpdated);
}

void WindowsAPI::GetAllDisplayInfo()
{
   LogInfo("GetAllDisplayInfo");
   DisplayDevices.clear();

   GetDisplayDevicesAndMonitorNames();
   const int numDisplays = DisplayDevices.size();

   if(numDisplays != NumDisplays)
   {
      NumDisplays = numDisplays;
      emit NumberOfDisplaysChanged(numDisplays);
   }

   EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(this));
}

void WindowsAPI::MonitorDatumReceived(HMONITOR hMonitor,
                                      HDC hdcMonitor,
                                      LPRECT lprcMonitor)
{
   Q_UNUSED(hdcMonitor)
   Q_UNUSED(lprcMonitor)

   LogInfo("MonitorDatumReceived");
   DisplayInfo info = GetDisplayInfo(hMonitor);

   if(DisplayInfo() != info)
   {
      emit DisplayDetected(info);
   }
}

DisplayInfo WindowsAPI::GetDisplayInfo(HMONITOR handle)
{
   MONITORINFO monitorInfo;
   monitorInfo.cbSize = sizeof(MONITORINFO);
   MONITORINFOEXA monitorInfoEx;
   monitorInfoEx.cbSize = sizeof(monitorInfoEx);
   if(GetMonitorInfo(handle, &monitorInfo) && GetMonitorInfo(handle, &monitorInfoEx))
   {
      const QString name = monitorInfoEx.szDevice;
      DisplayInfo& info = *(DisplayDevices.find(name.toStdString().c_str()));

      const RECT rect = monitorInfo.rcMonitor;
      info.Rect = {rect.left,
                   rect.top,
                   std::abs(rect.left - rect.right),
                   std::abs(rect.top - rect.bottom)};
      info.IsPrimary = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY);

      UINT dpiX, dpiY;
      if(S_OK == GetDpiForMonitor(handle,
                                   MDT_EFFECTIVE_DPI,
                                   &dpiX,
                                   &dpiY))
      {
         info.XDPI = dpiX;
         info.YDPI = dpiY;
      }

      LogInfo(QString("Matched handle to id %1")
                 .arg(info.Name));

      return info;
   }

   return DisplayInfo();
}

void WindowsAPI::GetDisplayDevicesAndMonitorNames()
{
   DISPLAY_DEVICEA displayDevice;
   displayDevice.cb = sizeof(displayDevice);
   for(int iter = 0; EnumDisplayDevicesA(NULL, iter, &displayDevice, 0); ++iter)
   {
      DisplayInfo info;
      info.Name = QString("Monitor %1").arg(iter);

      DISPLAY_DEVICEA displayDeviceForMonitorName;
      displayDeviceForMonitorName.cb = sizeof(displayDeviceForMonitorName);
      if(EnumDisplayDevicesA(displayDevice.DeviceName, 0, &displayDeviceForMonitorName, 0))
      {
         info.Name = displayDeviceForMonitorName.DeviceString;
      }

      info.Number = iter;
      DisplayDevices[displayDevice.DeviceName] = info;
      LogInfo(QString("Got DisplayDevice %1 with id %2 and name %3")
                 .arg(QString::number(iter), displayDevice.DeviceName, info.Name));
   }
}
