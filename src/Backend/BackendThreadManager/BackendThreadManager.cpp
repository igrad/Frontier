#include "BackendThreadManager.h"

#include <WallpaperService.h>

#include <QThread>

BackendThreadManager::BackendThreadManager()
   : TheWallpaperService(nullptr)
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

Wallpaper::WallpaperService* BackendThreadManager::GetTheWallpaperService()
{
   return TheWallpaperService;
}

void BackendThreadManager::HandleUIConnectedToServiceComponents()
{
}

void BackendThreadManager::HandleServiceThreadStarted()
{
   TheWallpaperService = new Wallpaper::WallpaperService(this);

   emit ServiceThreadStarted();
}
