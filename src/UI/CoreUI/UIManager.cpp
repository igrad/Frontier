#include "ShellWindow.h"
#include "UIManager.h"

#include <Wallpaper/WallpaperView.h>

UIManager::UIManager(XThread<DataAccessThreadManager> dataAccess,
                     XThread<BackendThreadManager> backend)
   : DataAccess(dataAccess)
   , Backend(backend)
   , TheShellWindow(nullptr)
   , TaskBarService(nullptr)
   , WallpaperService(nullptr)
   , TheWallpaperView(nullptr)
{
   connect(this, &UIManager::UIConnectedToServiceComponents,
           DataAccess.get(), &DataAccessThreadManager::HandleUIConnectedToComponents);
}

UIManager::~UIManager()
{
   if(TheShellWindow)
   {
      TheShellWindow->deleteLater();
      TheShellWindow = nullptr;
   }
}

void UIManager::Start()
{
   emit UIConnectedToServiceComponents();

   TheShellWindow->show();
}

void UIManager::HandleServiceThreadStarted()
{
   connect(this, &UIManager::RequestPassTaskBarService,
           Backend.get(), &BackendThreadManager::HandleRequestTaskBarService);
   connect(this, &UIManager::RequestPassWallpaperService,
           Backend.get(), &BackendThreadManager::HandleRequestWallpaperService);

   emit RequestPassWallpaperService();
   emit RequestPassTaskBarService();
}

void UIManager::HandlePassTaskBarService(XThread<TaskBar::TaskBarServiceInterface> service)
{
   TaskBarService = service;
   BuildUIComponents();
}

void UIManager::HandlePassWallpaperService(XThread<Wallpaper::WallpaperServiceInterface> service)
{
   WallpaperService = service;
   BuildUIComponents();
}

void UIManager::BuildUIComponents()
{
   if(!TaskBarService.isNull()&&
       !WallpaperService.isNull())
   {
      BuildTheShellWindow();

      // Build in z-index order
      BuildTheWallpaperView();
      // BuildTheTaskBarView();

      Start();
   }
}

void UIManager::BuildTheShellWindow()
{
   TheShellWindow = new ShellWindow();
   connect(TheShellWindow, &ShellWindow::Closed,
           this, &UIManager::ShellWindowClosed);
}

void UIManager::BuildTheWallpaperView()
{
   TheWallpaperView = new Wallpaper::WallpaperView(WallpaperService,
                                                   TheShellWindow);
}
