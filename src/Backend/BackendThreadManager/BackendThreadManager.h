#pragma once

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

class BackendThreadManager: public QObject
{
   Q_OBJECT

public:
   explicit BackendThreadManager(DataAccessThreadManager* dataAccess);
   ~BackendThreadManager() = default;

   void AssignToThread(QThread* thread);

   TaskBar::TaskBarServiceInterface* GetTheTaskBarService() const;
   Wallpaper::WallpaperServiceInterface* GetTheWallpaperService() const;

public slots:
   void HandleRequestPassWallpaperService();
   void HandleRequestPassTaskBarService();

signals:
   void ServiceThreadStarted();
   void PassWallpaperService(Wallpaper::WallpaperServiceInterface* service);
   void PassTaskBarService(TaskBar::TaskBarServiceInterface* service);

private slots:
   void HandleDataAccessThreadStarted();

private:
   Wallpaper::WallpaperServiceInterface* TheWallpaperService;
   TaskBar::TaskBarServiceInterface* TheTaskBarService;
};
