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
   void MonitorDatumReceived();

   static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
                                        HDC hdcMonitor,
                                        LPRECT lprcMonitor,
                                        LPARAM dwData);

private:
   static QList<HMONITOR> MONITOR_HANDLES;
   static int NUM_MONITORS;
   static WindowsAPI* INSTANCE;

   void ConnectToEventMessageFilter(const WindowsEventMessageFilter& filter);
   void GetAllDisplayInfo();
   DisplayInfo GetDisplayInfo(int index);

   QHash<Windows::Setting, QVariant> CachedSettings;
   QList<DisplayInfo> CachedDisplaysInfo;
};
