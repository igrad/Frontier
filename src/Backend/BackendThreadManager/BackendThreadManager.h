#pragma once

#include <QObject>

class DataAccessThreadManager;
class DisplaysManagerInterface;
class QApplication;

namespace TaskBar
{
   class TaskBarServiceInterface;
}

namespace Wallpaper
{
   class WallpaperServiceInterface;
}

class WindowsAPIInterface;
class WindowsEventMessageFilter;

class BackendThreadManager: public QObject
{
   Q_OBJECT

public:
   BackendThreadManager(DataAccessThreadManager* dataAccess,
                        QApplication* app);
   ~BackendThreadManager() = default;

   void AssignToThread(QThread* thread);

public slots:
   void HandleRequestPassDisplaysManager();
   void HandleRequestPassTaskBarService();
   void HandleRequestPassWallpaperService();
   void HandlePollDisplaysInfo();

signals:
   void ServiceThreadStarted();
   void PassDisplaysManager(DisplaysManagerInterface* service);
   void PassWallpaperService(Wallpaper::WallpaperServiceInterface* service);
   void PassTaskBarService(TaskBar::TaskBarServiceInterface* service);

private slots:
   void HandleDataAccessThreadStarted();

private:
   void CreateDisplaysManager();
   void CreateTaskBarService();
   void CreateWallpaperService();

   WindowsEventMessageFilter* WindowsEventFilter;
   WindowsAPIInterface* TheWindowsAPI;
   DisplaysManagerInterface* TheDisplaysManager;
   Wallpaper::WallpaperServiceInterface* TheWallpaperService;
   TaskBar::TaskBarServiceInterface* TheTaskBarService;
};
