#include "BackendThreadManager.h"

#include <TaskBarService.h>
#include <WallpaperService.h>

#include <QThread>

BackendThreadManager::BackendThreadManager()
   : TheWallpaperService(nullptr)
{
}

void BackendThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);

   connect(thread, &QThread::started,
           this, &BackendThreadManager::HandleServiceThreadStarted,
           Qt::UniqueConnection);
}

Wallpaper::WallpaperService* BackendThreadManager::GetTheWallpaperService()
{
   return TheWallpaperService;
}

void BackendThreadManager::HandleServiceThreadStarted()
{
   TheWallpaperService = new Wallpaper::WallpaperService(this);
   TheWallpaperService->RegisterMetaTypes();

   TheTaskBarService = new TaskBar::TaskBarService(this);
   TheTaskBarService->RegisterMetaTypes();

   emit ServiceThreadStarted();
}
