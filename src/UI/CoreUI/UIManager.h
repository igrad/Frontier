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
   ~UIManager();

signals:
   void UIConnectedToServiceComponents();
   void ShellWindowClosed();
   void PollDisplaysInfo();

private slots:
   void HandleDataAccessThreadStarted();
   void HandleServiceThreadStarted();
   void HandleDisplayConfigChanged(const DisplayEvent& event,
                                   const QMap<QString, DisplayInfo>& displays);
   void HandlePassDisplaysManager(DisplaysManagerInterface* manager);
   void HandlePassAssetLoader(Assets::AssetLoaderInterface* loader);
   void HandlePassTaskBarService(TaskBar::TaskBarServiceInterface* service);
   void HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service);

private:
   void Start();
   void BuildShellWindows();
   void RequestDisplaysInfo();

   XPtr<DataAccessThreadManager> DataAccess;
   XPtr<BackendThreadManager> Backend;
   XPtr<Assets::AssetLoaderInterface> TheAssetLoader;
   XPtr<DisplaysManagerInterface> DisplaysManager;
   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;
   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;

   Assets::AssetManager* TheAssetManager;

   QList<ShellUI*> Shells;
   QMap<QString, DisplayInfo> Displays;
   bool DisplaysInfoRequested;
   bool DisplaysInfoReceived;
};
