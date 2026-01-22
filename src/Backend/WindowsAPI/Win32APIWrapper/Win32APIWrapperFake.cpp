#include "Win32APIWrapperFake.h"

#include <Log.h>

QMap<int, int> Win32APIWrapperFake::SystemMetrics;
QMap<int, Win32APIWrapperFake::DisplayMonitor> Win32APIWrapperFake::DisplayMonitors;
QMap<int, MONITORINFOEXA> Win32APIWrapperFake::MonitorInfos;
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
   LogInfo(QString("Enum display monitor for device number %1").arg(iter));
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

   // Change the pointer type so that the assignment doesn't cause issues
   LPMONITORINFOEXA exaPtr = static_cast<LPMONITORINFOEXA>(lpmi);
   *exaPtr = MonitorInfos[monitorIndex];
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

   // If lpDevice is null, get the adapter name
   // Otherwise get the device name
   if(nullptr == lpDevice)
   {
      strcpy_s(lpDisplayDevice->DeviceString, 128, dev.AdapterName);
      strcpy_s(lpDisplayDevice->DeviceName, 128, dev.AdapterName);
   }
   else if((0 == iDevNum) && (strcmp(lpDevice, dev.AdapterName) == 0))
   {
      strcpy_s(lpDisplayDevice->DeviceString, 128, dev.DeviceName);
      strcpy_s(lpDisplayDevice->DeviceName, 128, dev.DeviceName);
   }
   else
   {
      LogError("Win32 fake failed to handle an EnumDisplayDevicesA input. Run gdb.");
      return false;
   }

   strcpy_s(lpDisplayDevice->DeviceID, 128, dev.DeviceID);
   lpDisplayDevice->StateFlags |= DISPLAY_DEVICE_ACTIVE;

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
   tagRECT& rect = *prect;
   QRect qrect = info.Rect;
   rect.bottom = qrect.bottom();
   rect.left = qrect.left();
   rect.top = qrect.top();
   rect.right = qrect.right();
   monitor.lproClip = prect;
   DisplayMonitors[num] = monitor;

   // MonitorInfos
   MONITORINFOEXA mie;
   mie.rcMonitor = rect;
   mie.rcWork = rect;
   strcpy_s(mie.szDevice, 32, info.DisplayName.toStdString().c_str());
   if(info.IsPrimary)
   {
      mie.dwFlags |= MONITORINFOF_PRIMARY;
   }
   MonitorInfos[num] = mie;

   // DisplayDevices
   DisplayDevice dev;
   dev.DeviceID = new char[128];
   dev.AdapterName = new char[128];
   dev.DeviceName = new char[128];
   strcpy_s(dev.DeviceID, 128, info.ID.toStdString().c_str());
   strcpy_s(dev.AdapterName, 128, info.DisplayName.toStdString().c_str());
   strcpy_s(dev.DeviceName, 128, info.DisplayName.toStdString().c_str());
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
