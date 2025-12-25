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

   connect(this, &UIManager::UIConnectedToServiceComponents,
Backend, &BackendThreadManager::HandleUIConnectedToServiceComponents,
Qt::UniqueConnection);
}

UIManager::~UIManager()
{
   TheShellWindow->deleteLater();
}

void UIManager::Start()
{
   emit UIConnectedToServiceComponents();

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
