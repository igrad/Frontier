#include "WindowsAPI.h"
#include "WindowsEventMessageFilter.h"

#include <DisplayInfo.h>

#include <Log.h>

#include <windows.h>
#include <shellscalingapi.h>

// NOTE: For now, the WindowsAPI class directly interfaces with windows.h. This isn't very
// testable, but it's fine for now. We will eventually need to make a 1:1 wrapper for the windows
// API so that we can mock it and test this thoroughly.

QList<HMONITOR> WindowsAPI::MONITOR_HANDLES;
WindowsAPI* WindowsAPI::INSTANCE = nullptr;
int WindowsAPI::NUM_MONITORS = 1;

WindowsAPI::WindowsAPI(const WindowsEventMessageFilter& filter,
                       QObject* parent)
   : CachedSettings()
{
   INSTANCE = this;
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

void WindowsAPI::HandleSettingUpdated(const Windows::Setting setting,
                                      const QVariant& value)
{
   Q_UNUSED(setting)
   Q_UNUSED(value)
}

void WindowsAPI::HandleDevicesChanged()
{
   GetAllDisplayInfo();
}

// TODO: Fix this static crap
BOOL CALLBACK WindowsAPI::MonitorEnumProc(HMONITOR hMonitor,
                                 HDC hdcMonitor,
                                 LPRECT lprcMonitor,
                                 LPARAM dwData)
{
   LogInfo("Received monitor");
   MONITOR_HANDLES.push_back(hMonitor);

   if(MONITOR_HANDLES.count() == NUM_MONITORS)
   {
      QMetaObject::invokeMethod(INSTANCE,
                                "MonitorDatumReceived");
   }
   return TRUE;
}

void WindowsAPI::ConnectToEventMessageFilter(const WindowsEventMessageFilter& filter)
{
   connect(&filter, &WindowsEventMessageFilter::SettingUpdated,
           this, &WindowsAPI::HandleSettingUpdated);
}

void WindowsAPI::GetAllDisplayInfo()
{
   LogInfo("GetAllDisplayInfo")
   MONITOR_HANDLES.clear();

   const int numDisplays = GetCurrentSettingValue(Windows::Setting::NumberOfDetectedMonitors)
                              .toInt();
   NUM_MONITORS = numDisplays;

   EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
}

void WindowsAPI::MonitorDatumReceived()
{
   LogInfo("MonitorDatumReceived");
   QList<DisplayInfo> infos;
   for(int iter = 0; iter < NUM_MONITORS; ++iter)
   {
      DisplayInfo info = GetDisplayInfo(iter);
      infos.push_back(info);
   }

   if(infos != CachedDisplaysInfo)
   {
      CachedDisplaysInfo = infos;

      emit DisplaysDetected(infos);
   }
}

DisplayInfo WindowsAPI::GetDisplayInfo(int index)
{
   DisplayInfo info;
   info.ID = index;

   MONITORINFO monitorInfo;
   LPMONITORINFO lpmi = &monitorInfo;
   lpmi->cbSize = sizeof(MONITORINFO);
   if(GetMonitorInfo(MONITOR_HANDLES[index], lpmi))
   {
      const RECT rect = monitorInfo.rcMonitor;
      info.Rect = {rect.left,
                   rect.top,
                   std::abs(rect.left - rect.right),
                   std::abs(rect.top - rect.bottom)};
      MONITORINFOF_PRIMARY;
      info.IsPrimary = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY);

      UINT dpiX, dpiY;
      if(S_OK == GetDpiForMonitor(MONITOR_HANDLES[index],
                                   MDT_EFFECTIVE_DPI,
                                   &dpiX,
                                   &dpiY))
      {
         info.XDPI = dpiX;
         info.YDPI = dpiY;
      }
   }

   return info;
}
