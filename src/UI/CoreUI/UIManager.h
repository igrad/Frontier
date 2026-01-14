#pragma once

#include <BackendThreadManager/BackendThreadManager.h>
#include <DataAccessThreadManager.h>

#include <Utilities/XPtr.h>

namespace Assets
{
   class AssetLoaderInterface;
   class AssetManager;
}

struct DisplayInfo;

class ShellUI;

class UIManager: public QObject
{
   Q_OBJECT

public:
   UIManager(DataAccessThreadManager* dataAccess,
             BackendThreadManager* backend);
   ~UIManager();

signals:
   void UIConnectedToServiceComponents();
   void ShellWindowClosed();

public slots:
   void HandleDisplaysInfo(const QList<DisplayInfo>& info);

private slots:
   void HandleDataAccessThreadStarted();
   void HandleServiceThreadStarted();
   void HandlePassAssetLoader(Assets::AssetLoaderInterface* loader);
   void HandlePassTaskBarService(TaskBar::TaskBarServiceInterface* service);
   void HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service);

private:
   void Start();
   void BuildShellWindows();

   XPtr<DataAccessThreadManager> DataAccess;
   XPtr<BackendThreadManager> Backend;
   XPtr<Assets::AssetLoaderInterface> TheAssetLoader;
   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;
   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;

   Assets::AssetManager* TheAssetManager;

   QList<ShellUI*> Shells;
   QList<DisplayInfo> DisplaysInfo;
   bool DisplaysInfoReceived;
};
