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

// Notes: Change DisplayEvent to have an event type mapped for each display ID
// That way we can quickly tell if we need to just remove a worker or add a new one, or do nothing
// I want to find a way to make an ID for DisplayInfo objects using the hardware as an identifier.
// This will be hard to do because the API doesn't easily offer much session-persistent info
// about displays, so I might have to come up with a hash of my own. Monitor name x rect or
// something like that. It will make the settings more annoying to work with in Enterprise (maybe
// we can find a way to work around that later once we try to set up Enterprise for in-Frontier
// use).
// Still WIP changing from displayNums to IDs everywhere. Figure out the session-persistent ID thing
// before wrapping that up.
void WindowsAPI::GetAllDisplayInfo()
{
   DisplayDevices.clear();

   GetDisplayDevicesAndMonitorNames();
   const int numDisplays = DisplayDevices.size();
   LogInfo(QString("Detected %1 active displays").arg(numDisplays));

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

   DisplayInfo info = GetDisplayInfo(hMonitor);

   if(DisplayInfo() != info)
   {
      emit DisplayDetected(info);
   }
}

DisplayInfo WindowsAPI::GetDisplayInfo(HMONITOR handle)
{
   MONITORINFOEXA monitorInfo;
   monitorInfo.cbSize = sizeof(MONITORINFOEXA);
   bool a = GetMonitorInfoA(handle, &monitorInfo);
   if(a)
   {
      const QString name = monitorInfo.szDevice;
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
      if(!(displayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE))
      {
         break;
      }

      DisplayInfo info;
      info.ID = displayDevice.DeviceName;

      DISPLAY_DEVICEA displayDeviceForMonitorName;
      displayDeviceForMonitorName.cb = sizeof(displayDeviceForMonitorName);
      if(EnumDisplayDevicesA(displayDevice.DeviceName, 0, &displayDeviceForMonitorName, 0))
      {
         info.Name = displayDeviceForMonitorName.DeviceString;
      }

      info.Number = iter;
      DisplayDevices[info.ID] = info;
   }
}
