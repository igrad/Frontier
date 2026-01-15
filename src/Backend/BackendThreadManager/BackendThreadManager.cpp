#include "BackendThreadManager.h"

#include <DataAccessThreadManager.h>

#include <TaskBarService.h>
#include <WallpaperService.h>

#include <QThread>

BackendThreadManager::BackendThreadManager(DataAccessThreadManager* dataAccess)
   : TheTaskBarService(nullptr)
   , TheWallpaperService(nullptr)
{
   connect(dataAccess, &DataAccessThreadManager::DataAccessThreadStarted,
           this, &BackendThreadManager::HandleDataAccessThreadStarted);
}

void BackendThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);
}

TaskBar::TaskBarServiceInterface* BackendThreadManager::GetTheTaskBarService() const
{
   return TheTaskBarService;
}

Wallpaper::WallpaperServiceInterface* BackendThreadManager::GetTheWallpaperService() const
{
   return TheWallpaperService;
}

void BackendThreadManager::HandleRequestPassTaskBarService()
{
   emit PassTaskBarService(TheTaskBarService);
}

void BackendThreadManager::HandleRequestPassWallpaperService()
{
   emit PassWallpaperService(TheWallpaperService);
}

void BackendThreadManager::HandleDataAccessThreadStarted()
{
   TheWallpaperService = new Wallpaper::WallpaperService(this);
   TheWallpaperService->RegisterMetaTypes();

   TheTaskBarService = new TaskBar::TaskBarService(this);
   TheTaskBarService->RegisterMetaTypes();

   emit ServiceThreadStarted();
}
