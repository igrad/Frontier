#pragma once

#include "Win32APIWrapperInterface.h"

class Win32APIWrapper: public Win32APIWrapperInterface
{
   Q_OBJECT

public:
   Win32APIWrapper() = default;
   ~Win32APIWrapper() = default;

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
};
