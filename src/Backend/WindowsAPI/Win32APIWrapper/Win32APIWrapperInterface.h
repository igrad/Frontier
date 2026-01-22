#pragma once

#include <QObject>

#include <windows.h>
#include <shellscalingapi.h>

class Win32APIWrapperInterface: public QObject
{
   Q_OBJECT

public:
   ~Win32APIWrapperInterface() = default;

   virtual int GetSystemMetrics(int nIndex) = 0;
   virtual WINBOOL EnumDisplayMonitors(HDC hdc,
                                       LPCRECT lproClip,
                                       MONITORENUMPROC lpfnEnum,
                                       LPARAM dwData) = 0;
   virtual WINBOOL GetMonitorInfoA(HMONITOR hMonitor, LPMONITORINFO lpmi) = 0;
   virtual HRESULT GetDpiForMonitor(HMONITOR hMonitor,
                                    MONITOR_DPI_TYPE dpiType,
                                    UINT* dpiX,
                                    UINT* dpiY) = 0;
   virtual WINBOOL EnumDisplayDevicesA(LPCSTR lpDevice,
                                       DWORD iDevNum,
                                       PDISPLAY_DEVICEA lpDisplayDevice,
                                       DWORD dwFlags) = 0;
};
