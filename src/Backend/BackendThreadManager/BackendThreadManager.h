#pragma once

#include <QObject>

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

   Wallpaper::WallpaperService* GetTheWallpaperService();

public slots:
   void HandleUIConnectedToServiceComponents();

signals:
   void ServiceThreadStarted();

private slots:
   void HandleServiceThreadStarted();

private:
   Wallpaper::WallpaperService* TheWallpaperService;
};
