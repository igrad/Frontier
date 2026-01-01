#pragma once

#include <Utilities/XThread.h>

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
   void HandleRequestWallpaperService();
   void HandleRequestTaskBarService();

signals:
   void ServiceThreadStarted();
   void PassWallpaperService(XThread<Wallpaper::WallpaperServiceInterface> service);
   void PassTaskBarService(XThread<TaskBar::TaskBarServiceInterface> service);

private slots:
   void HandleServiceThreadStarted();

private:
   Wallpaper::WallpaperServiceInterface* TheWallpaperService;
   TaskBar::TaskBarServiceInterface* TheTaskBarService;
};
