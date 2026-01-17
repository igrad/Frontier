#pragma once

#include <Utilities/XPtr.h>

#include <QObject>

class DataAccessThreadManager;

namespace TaskBar
{
   class TaskBarServiceInterface;
}

namespace Wallpaper
{
   class WallpaperServiceInterface;
}

class WindowsAPIInterface;

class BackendThreadManager: public QObject
{
   Q_OBJECT

public:
   BackendThreadManager(DataAccessThreadManager* dataAccess,
                        WindowsAPIInterface* windowsAPI);
   ~BackendThreadManager() = default;

   void AssignToThread(QThread* thread);

   TaskBar::TaskBarServiceInterface* GetTheTaskBarService() const;
   Wallpaper::WallpaperServiceInterface* GetTheWallpaperService() const;

public slots:
   void HandleRequestPassTaskBarService();
   void HandleRequestPassWallpaperService();

signals:
   void ServiceThreadStarted();
   void PassWallpaperService(Wallpaper::WallpaperServiceInterface* service);
   void PassTaskBarService(TaskBar::TaskBarServiceInterface* service);

private slots:
   void HandleDataAccessThreadStarted();

private:
   void CreateTaskBarService();
   void CreateWallpaperService();

   XPtr<WindowsAPIInterface> WindowsAPI;
   Wallpaper::WallpaperServiceInterface* TheWallpaperService;
   TaskBar::TaskBarServiceInterface* TheTaskBarService;
};
