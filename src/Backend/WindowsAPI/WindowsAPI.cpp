#include "WindowsAPI.h"
#include "WindowsEventMessageFilter.h"
#include "Win32APIWrapper/Win32APIWrapperFake.h"
#include "Win32APIWrapper/Win32APIWrapper.h"

#include <ArgParser.h>
#include <DisplayInfo.h>

#include <Log.h>

#include <Ntddvdeo.h>
#include <windows.h>
#include <shellscalingapi.h>

WindowsAPI::WindowsAPI(const WindowsEventMessageFilter& filter,
                       QObject* parent)
   : APIWrapper(nullptr)
   , CachedSettings()
{
   if(ArgParser::RunningWithEnterprise() || ArgParser::RunningUnitTests())
   {
      APIWrapper.reset(new Win32APIWrapperFake());
   }
   else
   {
      APIWrapper.reset(new Win32APIWrapper());
   }

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
         retVal = APIWrapper->GetSystemMetrics(SM_CMONITORS);
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
   CachedDisplaysInfo.clear();
   DisplayDevices.clear();

   GetDisplayDevicesAndMonitorNames();
   const int numDisplays = DisplayDevices.size();
   LogInfo(QString("Detected %1 active displays").arg(numDisplays));

   if(numDisplays != NumDisplays)
   {
      NumDisplays = numDisplays;
      emit NumberOfDisplaysChanged(numDisplays);
   }

   APIWrapper->EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(this));
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
   if(APIWrapper->GetMonitorInfoA(handle, &monitorInfo))
   {
      LogInfo("GetMonitorInfoA returned true");
      const QString name = monitorInfo.szDevice;
      DisplayInfo& info = *(DisplayDevices.find(name.toStdString().c_str()));

      const RECT rect = monitorInfo.rcMonitor;
      info.Rect = {rect.left,
                   rect.top,
                   std::abs(rect.left - rect.right),
                   std::abs(rect.top - rect.bottom)};
      info.IsPrimary = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY);

      UINT dpiX, dpiY;
      if(S_OK == APIWrapper->GetDpiForMonitor(handle,
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
   for(int iter = 0; APIWrapper->EnumDisplayDevicesA(NULL, iter, &displayDevice, 0); ++iter)
   {
      if(!(displayDevice.StateFlags & DISPLAY_DEVICE_ACTIVE))
      {
         break;
      }

      DisplayInfo info;
      info.SessionName = displayDevice.DeviceName;

      DISPLAY_DEVICEA displayDeviceForMonitorName;
      displayDeviceForMonitorName.cb = sizeof(displayDeviceForMonitorName);
      // The EDD_GET_DEVICE_INTERFACE_NAME flag populated the DeviceID field with the display EDID
      if(APIWrapper->EnumDisplayDevicesA(displayDevice.DeviceName,
                              0,
                              &displayDeviceForMonitorName,
                              EDD_GET_DEVICE_INTERFACE_NAME))
      {
         info.DisplayName = displayDeviceForMonitorName.DeviceString;
         info.ID = displayDeviceForMonitorName.DeviceID;
      }

      info.Number = GetDisplayNumberFromName(info.SessionName);
      if(UINT8_MAX == info.Number)
      {
         info.Number = iter;
      }
      DisplayDevices[info.SessionName] = info;
   }
}


int WindowsAPI::GetDisplayNumberFromName(const QString& name)
{
   int val = UINT8_MAX;

   QStringList tokens = name.split("\\");
   bool ok = false;
   if(tokens.size() >= 4)
   {
      tokens = tokens[3].split("DISPLAY");
      if(tokens.size() >= 2)
      {
         const int num = tokens[1].toInt(&ok);
         if(ok)
         {
            val = num;
         }
      }
   }

   return val;
}
