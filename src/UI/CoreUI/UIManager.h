#pragma once

#include <BackendThreadManager/BackendThreadManager.h>
#include <DataAccessThreadManager.h>

#include <Utilities/XThread.h>

class ShellWindow;

namespace Wallpaper
{
   class WallpaperView;
}

class UIManager: public QObject
{
   Q_OBJECT

public:
   explicit UIManager(XThread<DataAccessThreadManager> dataAccess,
                      XThread<BackendThreadManager> backend);
   ~UIManager();

signals:
   void UIConnectedToServiceComponents();
   void ShellWindowClosed();
   void RequestPassWallpaperService();
   void RequestPassTaskBarService();

private slots:
   void HandleServiceThreadStarted();
   void HandlePassTaskBarService(XThread<TaskBar::TaskBarServiceInterface> service);
   void HandlePassWallpaperService(XThread<Wallpaper::WallpaperServiceInterface> service);

private:
   void Start();

   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();

   XThread<DataAccessThreadManager> DataAccess;
   XThread<BackendThreadManager> Backend;
   ShellWindow* TheShellWindow;

   XThread<TaskBar::TaskBarServiceInterface> TaskBarService;

   XThread<Wallpaper::WallpaperServiceInterface> WallpaperService;
   Wallpaper::WallpaperView* TheWallpaperView;
};
