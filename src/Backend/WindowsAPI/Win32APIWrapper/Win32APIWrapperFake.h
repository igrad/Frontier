#pragma once

#include "Win32APIWrapperInterface.h"

#include <QMap>

class Win32APIWrapperFake: public Win32APIWrapperInterface
{
   Q_OBJECT

public:
   Win32APIWrapperFake() = default;
   ~Win32APIWrapperFake() = default;

   int GetSystemMetrics(int nIndex) override;
   WINBOOL EnumDisplayMonitors(HDC hdc,
                               LPCRECT lproClip,
                               MONITORENUMPROC lpfnEnum,
                               LPARAM dwData) override;
   WINBOOL GetMonitorInfoA(HMONITOR hMonitor, LPMONITORINFO lpmi) override;
   HRESULT GetDpiForMonitor(HMONITOR hMonitor,
                            MONITOR_DPI_TYPE dpiType,
                            UINT* dpiX,
                            UINT* dpiY) override;
   WINBOOL EnumDisplayDevicesA(LPCSTR lpDevice,
                               DWORD iDevNum,
                               PDISPLAY_DEVICEA lpDisplayDevice,
                               DWORD dwFlags) override;

   QMap<int, int> SystemMetrics;

   struct DisplayMonitor
   {
      HDC hdc;
      LPRECT lproClip;
      HMONITOR__ hMonitor;
   };
   QMap<int, DisplayMonitor> DisplayMonitors;
   QMap<int, MONITORINFOEX> MonitorInfos;

   struct DisplayDevice
   {
      LPCSTR DeviceID;
      LPCSTR MonitorName;
      LPCSTR DeviceName;
      DISPLAY_DEVICEA lpDisplayDevice;
   };
   QMap<int, DisplayDevice> DisplayDevices;

   QMap<int, QPair<UINT, UINT>> DPIs;

private:
   int GetIndexFromHandle(HMONITOR hMonitor);
};
