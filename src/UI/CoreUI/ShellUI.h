#pragma once

#include <DisplayInfo.h>

#include <Utilities/XPtr.h>

class ShellWindow;

namespace Assets
{
   class AssetManager;
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
   ShellUI(std::shared_ptr<Assets::AssetManager> assetManager,
           XPtr<TaskBar::TaskBarServiceInterface>& taskBarService,
           XPtr<Wallpaper::WallpaperServiceInterface>& wallpaperService,
           const DisplayInfo& displayInfo,
           QObject* parent = nullptr);
   ~ShellUI() = default;

   uint8_t GetDisplayID() const;
   DisplayInfo GetDisplayInfo() const;

signals:
   void ShellWindowClosed(uint8_t ID);

public slots:
   void HandleDisplayInfoUpdated(const DisplayInfo& info);

private:
   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();
   void BuildTheTaskBarView();
   void Start();

   std::shared_ptr<Assets::AssetManager> TheAssetManager;
   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;
   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;
   ShellWindow* TheShellWindow;
   Wallpaper::WallpaperView* TheWallpaperView;
   TaskBar::TaskBarView* TheTaskBarView;
   DisplayInfo TheDisplayInfo;
};
