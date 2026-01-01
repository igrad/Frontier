#pragma once

#include <BackendThreadManager/BackendThreadManager.h>
#include <DataAccessThreadManager.h>

#include <Utilities/XPtr.h>

class AssetLoaderInterface;
class AssetManager;
class ShellWindow;

namespace Wallpaper
{
   class WallpaperView;
}

class UIManager: public QObject
{
   Q_OBJECT

public:
   explicit UIManager(DataAccessThreadManager* dataAccess,
                      BackendThreadManager* backend);
   ~UIManager();

signals:
   void UIConnectedToServiceComponents();
   void ShellWindowClosed();

private slots:
   void HandleDataAccessThreadStarted();
   void HandleServiceThreadStarted();
   void HandlePassAssetLoader(AssetLoaderInterface* loader);
   void HandlePassTaskBarService(TaskBar::TaskBarServiceInterface* service);
   void HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service);

private:
   void Start();

   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();

   XPtr<DataAccessThreadManager> DataAccess;
   XPtr<BackendThreadManager> Backend;
   ShellWindow* TheShellWindow;

   XPtr<AssetLoaderInterface> TheAssetLoader;
   AssetManager* TheAssetManager;

   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;

   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;
   Wallpaper::WallpaperView* TheWallpaperView;
};
