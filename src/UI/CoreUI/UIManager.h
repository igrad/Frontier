#pragma once

#include <BackendThreadManager/BackendThreadManager.h>
#include <DataAccessThreadManager.h>

#include <DisplaysManager/DisplayInfo.h>

#include <Utilities/XPtr.h>

#include <QMap>

namespace Assets
{
   class AssetLoaderInterface;
   class AssetManager;
}

class DisplaysManagerInterface;
class ShellUI;
class WindowsAPIInterface;

class UIManager: public QObject
{
   Q_OBJECT

public:
   UIManager(DataAccessThreadManager* dataAccess,
             BackendThreadManager* backend);
   ~UIManager() = default;

signals:
   void UIConnectedToServiceComponents();
   void ShellWindowClosed();
   void PollDisplaysInfo();

private slots:
   void HandleDataAccessThreadStarted();
   void HandleServiceThreadStarted();
   void HandleDisplayConfigChanged(const DisplayConfigEvent& event);
   void HandlePassDisplaysManager(DisplaysManagerInterface* manager);
   void HandlePassAssetLoader(Assets::AssetLoaderInterface* loader);
   void HandlePassTaskBarService(TaskBar::TaskBarServiceInterface* service);
   void HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service);

private:
   void Start();
   void BuildShellWindow(const DisplayInfo& info);
   void RemoveShellWindow(const DisplayInfo& info);
   void RequestDisplaysInfo();

   XPtr<DataAccessThreadManager> DataAccess;
   XPtr<BackendThreadManager> Backend;
   XPtr<Assets::AssetLoaderInterface> TheAssetLoader;
   XPtr<DisplaysManagerInterface> DisplaysManager;
   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;
   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;

   Assets::AssetManager* TheAssetManager;

   QMap<DisplayID, ShellUI*> Shells;
   bool DisplaysInfoRequested;
   bool DisplaysInfoReceived;
};
