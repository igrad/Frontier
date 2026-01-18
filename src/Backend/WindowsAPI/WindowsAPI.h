#pragma once

#include "WindowsAPIInterface.h"

#include <windows.h>

struct DisplayInfo;
class WindowsEventMessageFilter;

class WindowsAPI: public WindowsAPIInterface
{
   Q_OBJECT

public:
   WindowsAPI(const WindowsEventMessageFilter& filter,
              QObject* parent = nullptr);
   ~WindowsAPI() = default;

   QVariant GetCurrentSettingValue(Windows::Setting setting) override;

public slots:
   void HandlePollDisplaysInfo() override;

private slots:
   void HandleWindowsSettingUpdated(const Windows::Setting setting,
                             const QVariant& value);
   void HandleDevicesChanged();
   void MonitorDatumReceived(HMONITOR hMonitor,
                             HDC hdcMonitor,
                             LPRECT lprcMonitor);

   static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
                                        HDC hdcMonitor,
                                        LPRECT lprcMonitor,
                                        LPARAM dwData);

private:
   void ConnectToEventMessageFilter(const WindowsEventMessageFilter& filter);
   void GetAllDisplayInfo();
   DisplayInfo GetDisplayInfo(HMONITOR handle);
   void GetDisplayDevicesAndMonitorNames();

   QHash<Windows::Setting, QVariant> CachedSettings;
   uint8_t NumDisplays;
   QHash<HMONITOR, DisplayInfo> CachedDisplaysInfo;
   QMap<QString, DisplayInfo> DisplayDevices;
};
