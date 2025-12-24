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

std::unique_ptr<Settings::SettingsServiceInterface>& BackendThreadManager::GetTheSettingsService()
{
   return TheSettingsService;
}

std::unique_ptr<Wallpaper::WallpaperService>& BackendThreadManager::GetTheWallpaperService()
{
   return TheWallpaperService;
}

void BackendThreadManager::HandleServiceThreadStarted()
{
   TheSettingsService.reset(new Settings::SettingsService());
   TheWallpaperService.reset(new Wallpaper::WallpaperService());
}
