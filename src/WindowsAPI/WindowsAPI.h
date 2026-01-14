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

private slots:
   void HandleSettingUpdated(const Windows::Setting setting,
                             const QVariant& value);
   void HandleDevicesChanged();

private:
   static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
                                        HDC hdcMonitor,
                                        LPRECT lprcMonitor,
                                        LPARAM dwData);
   static QList<HMONITOR> MONITOR_HANDLES;

   void ConnectToEventMessageFilter(const WindowsEventMessageFilter& filter);
   void GetAllDisplayInfo();
   DisplayInfo GetDisplayInfo(int index);

   QHash<Windows::Setting, QVariant> CachedSettings;
   QList<DisplayInfo> CachedDisplaysInfo;
};
