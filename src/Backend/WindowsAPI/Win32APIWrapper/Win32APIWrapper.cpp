#include "Win32APIWrapper.h"

int Win32APIWrapper::GetSystemMetrics(int nIndex)
{
   return ::GetSystemMetrics(nIndex);
}

WINBOOL Win32APIWrapper::EnumDisplayMonitors(HDC hdc,
                                             LPCRECT lproClip,
                                             MONITORENUMPROC lpfnEnum,
                                             LPARAM dwData)
{
   return ::EnumDisplayMonitors(hdc, lproClip, lpfnEnum, dwData);
}

WINBOOL Win32APIWrapper::GetMonitorInfoA(HMONITOR hMonitor, LPMONITORINFO lpmi)
{
   return ::GetMonitorInfoA(hMonitor, lpmi);
}

HRESULT Win32APIWrapper::GetDpiForMonitor(HMONITOR hMonitor,
                                          MONITOR_DPI_TYPE dpiType,
                                          UINT* dpiX,
                                          UINT* dpiY)
{
   return ::GetDpiForMonitor(hMonitor, dpiType, dpiX, dpiY);
}

WINBOOL Win32APIWrapper::EnumDisplayDevicesA(LPCSTR lpDevice,
                                             DWORD iDevNum,
                                             PDISPLAY_DEVICEA lpDisplayDevice,
                                             DWORD dwFlags)
{
   return ::EnumDisplayDevicesA(lpDevice, iDevNum, lpDisplayDevice, dwFlags);
}
