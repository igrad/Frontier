#include "ShellWindow.h"
#include "UIManager.h"

#include <Wallpaper/WallpaperView.h>
#include <BackendThreadManager/BackendThreadManager.h>

UIManager::UIManager(std::unique_ptr<BackendThreadManager>& backend)
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
   TheShellWindow.reset(new ShellWindow());
}

void UIManager::BuildTheWallpaperView()
{
   TheWallpaperView.reset(new Wallpaper::WallpaperView(Backend->GetTheWallpaperService()));
}
