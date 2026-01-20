#include "Win32APIWrapperFake.h"

#include <Log.h>

int Win32APIWrapperFake::GetSystemMetrics(int nIndex)
{
   if(SystemMetrics.contains(nIndex))
   {
      return SystemMetrics[nIndex];
   }

   LogWarn(QString("No SystemMetric preconfigured for nIndex %1")
              .arg(nIndex));
   return 0;
}

WINBOOL Win32APIWrapperFake::EnumDisplayMonitors(HDC hdc,
                            LPCRECT lproClip,
                            MONITORENUMPROC lpfnEnum,
                            LPARAM dwData)
{
   static int iter = 0;

   // You can implement these yourself if you need them
   Q_UNUSED(hdc)
   Q_UNUSED(lproClip)
   const bool cont = lpfnEnum(&DisplayMonitors[iter].hMonitor,
                              DisplayMonitors[iter].hdc,
                              DisplayMonitors[iter].lproClip,
                              dwData);

   if(cont)
   {
      ++iter;
      EnumDisplayMonitors(hdc, lproClip, lpfnEnum, dwData);
   }
   else
   {
      iter = 0;
   }

   return cont;
}

WINBOOL Win32APIWrapperFake::GetMonitorInfoA(HMONITOR hMonitor, LPMONITORINFO lpmi)
{
   const int monitorIndex = GetIndexFromHandle(hMonitor);

   if(!MonitorInfos.contains(monitorIndex))
   {
      LogError(QString("Could not locate monitor info from index %1")
                  .arg(monitorIndex));
      return false;
   }

   *lpmi = MonitorInfos[monitorIndex];
   return true;
}

HRESULT Win32APIWrapperFake::GetDpiForMonitor(HMONITOR hMonitor,
                         MONITOR_DPI_TYPE dpiType,
                         UINT* dpiX,
                         UINT* dpiY)
{
   const int monitorIndex = GetIndexFromHandle(hMonitor);

   if(!DPIs.contains(monitorIndex))
   {
      LogError(QString("Could not locate DPI info from index %1")
                  .arg(monitorIndex));
      return 0;
   }

   *dpiX = DPIs[monitorIndex].first;
   *dpiY = DPIs[monitorIndex].second;
   return 1;
}

WINBOOL Win32APIWrapperFake::EnumDisplayDevicesA(LPCSTR lpDevice,
                            DWORD iDevNum,
                            PDISPLAY_DEVICEA lpDisplayDevice,
                            DWORD dwFlags)
{
   if(!DisplayDevices.contains(iDevNum))
   {
      LogError(QString("Could not find DisplayDevice for iDevNum %1")
                  .arg(iDevNum));
      return false;
   }

   DisplayDevice& dev = DisplayDevices[iDevNum];

   // If lpDevice is null, get the whatever the device name
   if(nullptr == lpDevice)
   {
      strcpy_s(dev.lpDisplayDevice.DeviceID, 128, dev.DeviceName);
      dev.lpDisplayDevice.StateFlags &= DISPLAY_DEVICE_ACTIVE;
   }
   else if(0 == iDevNum)
   {
      strcpy_s(dev.lpDisplayDevice.DeviceID, 128, dev.DeviceID);
      dev.lpDisplayDevice.StateFlags &= DISPLAY_DEVICE_ACTIVE;
      strcpy_s(dev.lpDisplayDevice.DeviceString, 128, dev.DeviceName);
   }

   *lpDisplayDevice = dev.lpDisplayDevice;
   return true;
}

int Win32APIWrapperFake::GetIndexFromHandle(HMONITOR hMonitor)
{
   const auto iter = std::find_if(DisplayMonitors.constBegin(),
                                  DisplayMonitors.constEnd(),
                                  [&](const DisplayMonitor& monitor){
                                     return &monitor.hMonitor == hMonitor;
                                  });

   if(DisplayMonitors.constEnd() == iter)
   {
      LogError(QString("Could not locate monitor info from handle %1")
                  .arg(std::intptr_t(hMonitor)));
      return 0;
   }

   return iter.key();
}
