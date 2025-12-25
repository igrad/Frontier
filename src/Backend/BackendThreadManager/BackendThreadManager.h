#pragma once

#include <QObject>

namespace Settings
{
class SettingsServiceInterface;
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
   ~BackendThreadManager();

   void AssignToThread(QThread* thread);

   Settings::SettingsServiceInterface* GetTheSettingsService();
   Wallpaper::WallpaperService* GetTheWallpaperService();

public slots:
   void HandleUIConnectedToServiceComponents();

private slots:
   void HandleServiceThreadStarted();

private:
   Settings::SettingsServiceInterface* TheSettingsService;
   Wallpaper::WallpaperService* TheWallpaperService;
};
