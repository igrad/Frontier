#pragma once

#include "Win32APIWrapperInterface.h"

#include <DisplayInfo.h>

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

public slots:
   void HandleENTERPRISE_DisplayInfoModified(const DisplayConfigEvent& event);

private:
   struct DisplayDevice
   {
      LPSTR DeviceID;
      LPSTR AdapterName;
      LPSTR DeviceName;
   };
   static QMap<int, DisplayDevice> DisplayDevices;

   struct DisplayMonitor
   {
      HDC hdc;
      LPRECT lproClip;
      HMONITOR__ hMonitor;
   };
   static QMap<int, DisplayMonitor> DisplayMonitors;
   static QMap<int, QPair<UINT, UINT>> DPIs;
   static QMap<int, MONITORINFOEXA> MonitorInfos;
   static QMap<int, int> SystemMetrics;

   void AddDisplay(const DisplayInfo& info);
   int GetIndexFromHandle(HMONITOR hMonitor);
};
