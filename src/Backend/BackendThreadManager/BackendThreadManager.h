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

   std::unique_ptr<Settings::SettingsServiceInterface>& GetTheSettingsService();
   std::unique_ptr<Wallpaper::WallpaperService>& GetTheWallpaperService();

private slots:
   void HandleServiceThreadStarted();

private:
   std::unique_ptr<Settings::SettingsServiceInterface> TheSettingsService;
   std::unique_ptr<Wallpaper::WallpaperService> TheWallpaperService;
};
