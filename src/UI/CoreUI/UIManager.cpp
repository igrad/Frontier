#include "ShellWindow.h"
#include "UIManager.h"

#include <Wallpaper/WallpaperView.h>
#include <BackendThreadManager/BackendThreadManager.h>
#include <DataAccess/DataAccessThreadManager.h>

#include <iostream>

UIManager::UIManager(DataAccessThreadManager* dataAccess,
                     BackendThreadManager* backend)
   : DataAccess(dataAccess)
   , Backend(backend)
   , TheShellWindow(nullptr)
   , TheWallpaperView(nullptr)
{
   connect(this, &UIManager::UIConnectedToServiceComponents,
           DataAccess, &DataAccessThreadManager::HandleUIConnectedToComponents,
           Qt::UniqueConnection);
   connect(Backend, &BackendThreadManager::ServiceThreadStarted,
           this, &UIManager::HandleServiceThreadStarted);
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
   std::cout << "HandleServiceThreadStarted" <<  std::endl;
   BuildUIComponents();
   Start();
}

void UIManager::BuildUIComponents()
{
   BuildTheShellWindow();
   BuildTheWallpaperView();
}

void UIManager::BuildTheShellWindow()
{
   TheShellWindow = new ShellWindow();
   connect(TheShellWindow, &ShellWindow::Closed,
           this, &UIManager::ShellWindowClosed);
}

void UIManager::BuildTheWallpaperView()
{
   TheWallpaperView = new Wallpaper::WallpaperView(Backend->GetTheWallpaperService(),
                                                   TheShellWindow);
}
