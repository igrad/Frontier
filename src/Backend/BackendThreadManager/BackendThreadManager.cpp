#include "BackendThreadManager.h"

#include <TaskBarService.h>
#include <WallpaperService.h>

#include <QThread>

BackendThreadManager::BackendThreadManager()
   : TheTaskBarService(nullptr)
   , TheWallpaperService(nullptr)
{
}

void BackendThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);

   connect(thread, &QThread::started,
           this, &BackendThreadManager::HandleServiceThreadStarted,
           Qt::UniqueConnection);
}

TaskBar::TaskBarServiceInterface* BackendThreadManager::GetTheTaskBarService() const
{
   return TheTaskBarService;
}

Wallpaper::WallpaperServiceInterface* BackendThreadManager::GetTheWallpaperService() const
{
   return TheWallpaperService;
}

void BackendThreadManager::HandleRequestTaskBarService()
{
   emit PassTaskBarService(TheTaskBarService);
}

void BackendThreadManager::HandleRequestWallpaperService()
{
   emit PassWallpaperService(TheWallpaperService);
}

void BackendThreadManager::HandleServiceThreadStarted()
{
   TheWallpaperService = new Wallpaper::WallpaperService(this);
   TheWallpaperService->RegisterMetaTypes();

   TheTaskBarService = new TaskBar::TaskBarService(this);
   TheTaskBarService->RegisterMetaTypes();

   emit ServiceThreadStarted();
}
