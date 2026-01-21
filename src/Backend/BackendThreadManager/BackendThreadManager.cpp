#include "BackendThreadManager.h"

#include <DataAccessThreadManager.h>
#include <DisplaysManager.h>
#include <TaskBarService.h>
#include <WallpaperService.h>
#include <WindowsAPI.h>
#include <WindowsEventMessageFilter.h>

#include <QApplication>
#include <QThread>

BackendThreadManager::BackendThreadManager(DataAccessThreadManager* dataAccess,
                                           QApplication* app)
   : WindowsEventFilter(new WindowsEventMessageFilter(this))
   , TheWindowsAPI(new WindowsAPI(*WindowsEventFilter))
   , TheDisplaysManager()
   , TheTaskBarService(nullptr)
   , TheWallpaperService(nullptr)
{
   app->installNativeEventFilter(WindowsEventFilter);

   connect(dataAccess, &DataAccessThreadManager::DataAccessThreadStarted,
           this, &BackendThreadManager::HandleDataAccessThreadStarted);
   connect(this, &BackendThreadManager::ENTERPRISE_DisplayInfoModified,
           TheWindowsAPI, &WindowsAPIInterface::ENTERPRISE_DisplayInfoModified);
}

void BackendThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);
}

void BackendThreadManager::HandleRequestPassDisplaysManager()
{
   emit PassDisplaysManager(TheDisplaysManager);
}

void BackendThreadManager::HandleRequestPassTaskBarService()
{
   emit PassTaskBarService(TheTaskBarService);
}

void BackendThreadManager::HandleRequestPassWallpaperService()
{
   emit PassWallpaperService(TheWallpaperService);
}

void BackendThreadManager::HandlePollDisplaysInfo()
{
   TheDisplaysManager->PollDisplaysInfo();
}

void BackendThreadManager::HandleDataAccessThreadStarted()
{
   LogInfo("Handling DataAccess thread started");

   CreateDisplaysManager();
   CreateWallpaperService();
   CreateTaskBarService();

   emit ServiceThreadStarted();
}

void BackendThreadManager::CreateDisplaysManager()
{
   TheDisplaysManager = new DisplaysManager(*TheWindowsAPI, this);
   TheDisplaysManager->RegisterMetaTypes();
}

void BackendThreadManager::CreateTaskBarService()
{
   TheTaskBarService = new TaskBar::TaskBarService(this);
   TheTaskBarService->RegisterMetaTypes();

   // TODO Connect tot displays manager
}

void BackendThreadManager::CreateWallpaperService()
{
   TheWallpaperService = new Wallpaper::WallpaperService(this);
   TheWallpaperService->RegisterMetaTypes();

   connect(TheDisplaysManager, &DisplaysManagerInterface::DisplayConfigChanged,
           TheWallpaperService, &Wallpaper::WallpaperServiceInterface::HandleDisplayConfigChanged);
}
