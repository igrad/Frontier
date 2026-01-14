#pragma once

#include <DisplayInfo.h>

#include <Utilities/XPtr.h>

class ShellWindow;

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
   ShellUI(XPtr<TaskBar::TaskBarServiceInterface>& taskBarService,
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

   XPtr<TaskBar::TaskBarServiceInterface> TaskBarService;
   XPtr<Wallpaper::WallpaperServiceInterface> WallpaperService;
   ShellWindow* TheShellWindow;
   Wallpaper::WallpaperView* TheWallpaperView;
   TaskBar::TaskBarView* TheTaskBarView;
   DisplayInfo TheDisplayInfo;
};
