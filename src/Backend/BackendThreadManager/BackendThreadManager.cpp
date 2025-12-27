#include "BackendThreadManager.h"

#include <SettingsService.h>
#include <WallpaperService.h>

#include <QThread>

BackendThreadManager::BackendThreadManager()
   : TheSettingsService(nullptr)
   , TheWallpaperService(nullptr)
{
}

BackendThreadManager::~BackendThreadManager()
{
}

void BackendThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);

   connect(thread, &QThread::started,
           this, &BackendThreadManager::HandleServiceThreadStarted,
           Qt::UniqueConnection);
   connect(thread, &QThread::finished,
           this, &QObject::deleteLater,
           Qt::UniqueConnection);
}

Settings::SettingsServiceInterface* BackendThreadManager::GetTheSettingsService()
{
   return TheSettingsService;
}

Wallpaper::WallpaperService* BackendThreadManager::GetTheWallpaperService()
{
   return TheWallpaperService;
}

void BackendThreadManager::HandleUIConnectedToServiceComponents()
{
   TheSettingsService->FetchAllSettings();
}

void BackendThreadManager::HandleServiceThreadStarted()
{
   TheSettingsService = new Settings::SettingsService(this);
   TheWallpaperService = new Wallpaper::WallpaperService(this);

   emit ServiceThreadStarted();
}
