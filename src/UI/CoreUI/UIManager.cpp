#include "ShellWindow.h"
#include "UIManager.h"

#include <Wallpaper/WallpaperView.h>
#include <BackendThreadManager/BackendThreadManager.h>

UIManager::UIManager(BackendThreadManager* backend)
   : Backend(backend)
   , TheShellWindow(nullptr)
   , TheWallpaperView(nullptr)
{
   BuildUIComponents();
}

UIManager::~UIManager()
{
   TheShellWindow->deleteLater();
}

void UIManager::Start()
{
   TheShellWindow->show();
}

void UIManager::BuildUIComponents()
{
   BuildTheShellWindow();
   BuildTheWallpaperView();
}

void UIManager::BuildTheShellWindow()
{
   TheShellWindow = new ShellWindow();
}

void UIManager::BuildTheWallpaperView()
{
   TheWallpaperView = new Wallpaper::WallpaperView(Backend->GetTheWallpaperService(),
                                                   TheShellWindow);
}
