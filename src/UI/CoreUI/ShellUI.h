#pragma once

#include <DisplayInfo.h>

#include <Utilities/XPtr.h>

class ShellWindow;

namespace Assets
{
   class AssetClient;
}

namespace TaskBar
{
   class TaskBarServiceInterface;
   class TaskBarView;
}

namespace Wallpaper
{
   class WallpaperServiceInterface;
   class WallpaperView;
}

class ShellUI: public QObject
{
   Q_OBJECT

public:
   ShellUI(XPtr<TaskBar::TaskBarServiceInterface> taskBarService,
           XPtr<Wallpaper::WallpaperServiceInterface> wallpaperService,
           const DisplayInfo& displayInfo,
           QObject* parent = nullptr);
   ~ShellUI();

   DisplayID GetDisplayID() const;
   DisplayInfo GetDisplayInfo() const;

signals:
   void ShellWindowClosed(const DisplayID& displayID);

public slots:
   void HandleDisplayInfoChanged(const DisplayInfo& info);
   void HandleDisplayRemoved(const DisplayInfo& info);

private:
   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();
   void BuildTheTaskBarView();
   void Start();

   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;
   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;
   ShellWindow* TheShellWindow;
   Wallpaper::WallpaperView* TheWallpaperView;

   Assets::AssetClient* TaskBarAssetClient;
   TaskBar::TaskBarView* TheTaskBarView;
   DisplayInfo TheDisplayInfo;
};
