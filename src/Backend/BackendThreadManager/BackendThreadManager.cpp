#include "BackendThreadManager.h"

#include <WindowsAPI/WindowsAPIInterface.h>

#include <DataAccessThreadManager.h>

#include <TaskBarService.h>
#include <WallpaperService.h>

#include <QThread>

BackendThreadManager::BackendThreadManager(DataAccessThreadManager* dataAccess,
                                           WindowsAPIInterface* windowsAPI)
   : WindowsAPI(windowsAPI)
   , TheTaskBarService(nullptr)
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
   LogInfo("Handling DataAccess thread started");

   CreateWallpaperService();
   CreateTaskBarService();

   emit ServiceThreadStarted();
}

void BackendThreadManager::CreateTaskBarService()
{
   TheTaskBarService = new TaskBar::TaskBarService(this);
   TheTaskBarService->RegisterMetaTypes();
}

void BackendThreadManager::CreateWallpaperService()
{
   TheWallpaperService = new Wallpaper::WallpaperService(this);
   TheWallpaperService->RegisterMetaTypes();

   connect(WindowsAPI.get(), &WindowsAPIInterface::DisplaysDetected,
           TheWallpaperService, &Wallpaper::WallpaperServiceInterface::HandleDisplaysDetected);
}
