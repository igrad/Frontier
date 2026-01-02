#pragma once

#include <QObject>

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
   BackendThreadManager();
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
   void HandleServiceThreadStarted();

private:
   Wallpaper::WallpaperServiceInterface* TheWallpaperService;
   TaskBar::TaskBarServiceInterface* TheTaskBarService;
};
