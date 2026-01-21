#include "Win32APIWrapperFake.h"

#include <Log.h>

QMap<int, int> Win32APIWrapperFake::SystemMetrics;
QMap<int, Win32APIWrapperFake::DisplayMonitor> Win32APIWrapperFake::DisplayMonitors;
QMap<int, MONITORINFOEX> Win32APIWrapperFake::MonitorInfos;
QMap<int, Win32APIWrapperFake::DisplayDevice> Win32APIWrapperFake::DisplayDevices;
QMap<int, QPair<UINT, UINT>> Win32APIWrapperFake::DPIs;

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
   bool cont = (iter < DisplayMonitors.size()) &&
               lpfnEnum(&DisplayMonitors[iter].hMonitor,
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
      dev.lpDisplayDevice.StateFlags |= DISPLAY_DEVICE_ACTIVE;
   }
   else if(0 == iDevNum)
   {
      strcpy_s(dev.lpDisplayDevice.DeviceID, 128, dev.DeviceID);
      dev.lpDisplayDevice.StateFlags |= DISPLAY_DEVICE_ACTIVE;
      strcpy_s(dev.lpDisplayDevice.DeviceString, 128, dev.DeviceName);
   }

   *lpDisplayDevice = dev.lpDisplayDevice;
   return true;
}

void Win32APIWrapperFake::HandleENTERPRISE_DisplayInfoModified(const DisplayConfigEvent& event)
{
   LogInfo(QString("Fake received display config event with %1 displays")
              .arg(event.Displays.size()));
   DisplayMonitors.clear();
   MonitorInfos.clear();
   DisplayDevices.clear();
   DPIs.clear();

   for(const QPair<DisplayConfigEventType, DisplayInfo>& pair : event.Displays)
   {
      switch(pair.first)
      {
      case DisplayConfigEventType::Added:
      case DisplayConfigEventType::Changed:
         AddDisplay(pair.second);
         break;
      default:
         // No
         break;
      }
   }

   SystemMetrics[SM_CMONITORS] = DisplayMonitors.size();
}

void Win32APIWrapperFake::AddDisplay(const DisplayInfo& info)
{
   // Convert from Enterprise using 1-index display numbers to Frontier's 0-indexing
   // Wahoo
   int num = info.Number - 1;
   LogInfo(QString("Adding display %1 to fake").arg(num));

   // DisplayMonitors
   DisplayMonitor monitor;
   monitor.hMonitor = *info.Handle;
   LPRECT prect = new tagRECT();
   tagRECT rect = *prect;
   QRect qrect = info.Rect;
   rect.bottom = qrect.bottom();
   rect.left = qrect.left();
   rect.top = qrect.top();
   rect.right = qrect.right();
   monitor.lproClip = prect;
   DisplayMonitors[num] = monitor;

   // MonitorInfos
   MONITORINFOEX mie;
   mie.rcMonitor = rect;
   mie.rcWork = rect;
   wcscpy(mie.szDevice, info.DisplayName.toStdWString().c_str());
   if(info.IsPrimary)
   {
      mie.dwFlags |= MONITORINFOF_PRIMARY;
   }
   MonitorInfos[num] = mie;

   // DisplayDevices
   DisplayDevice dev;
   dev.DeviceID = info.ID.toStdString().c_str();
   dev.MonitorName = info.DisplayName.toStdString().c_str();
   dev.DeviceName = info.DisplayName.toStdString().c_str();
   DisplayDevices[num] = dev;

   // DPIs
   DPIs[num] = {info.XDPI, info.YDPI};
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
