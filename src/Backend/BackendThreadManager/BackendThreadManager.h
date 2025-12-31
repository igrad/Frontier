#pragma once

#include <QObject>

namespace TaskBar
{
class TaskBarServiceInterface;
}
namespace Wallpaper
{
class WallpaperService;
}

class BackendThreadManager: public QObject
{
   Q_OBJECT

public:
   BackendThreadManager();
   ~BackendThreadManager() = default;

   void AssignToThread(QThread* thread);

   Wallpaper::WallpaperService* GetTheWallpaperService();
   TaskBar::TaskBarServiceInterface* GetTheTaskBarService();

signals:
   void ServiceThreadStarted();

private slots:
   void HandleServiceThreadStarted();

private:
   Wallpaper::WallpaperService* TheWallpaperService;
   TaskBar::TaskBarServiceInterface* TheTaskBarService;
};
