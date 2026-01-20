#include "ShellUI.h"
#include "ShellWindow.h"

#include "TaskBar/TaskBarView.h"
#include "Wallpaper/WallpaperView.h"

#include <Log.h>

#include <AssetManager.h>

ShellUI::ShellUI(XPtr<TaskBar::TaskBarServiceInterface> taskBarService,
                 XPtr<Wallpaper::WallpaperServiceInterface> wallpaperService,
                 const DisplayInfo& displayInfo,
                 QObject* parent)
   : QObject(parent)
   , TaskBarService(taskBarService)
   , WallpaperService(wallpaperService)
   , TheShellWindow(nullptr)
   , TheWallpaperView(nullptr)
   , TheTaskBarView(nullptr)
   , TheDisplayInfo(displayInfo)
{
   BuildUIComponents();
}

ShellUI::~ShellUI()
{
   if(nullptr != TheShellWindow)
   {
      TheShellWindow->close();
      TheShellWindow->deleteLater();
      TheShellWindow = nullptr;
   }
}

DisplayID ShellUI::GetDisplayID() const
{
   return TheDisplayInfo.ID;
}

DisplayInfo ShellUI::GetDisplayInfo() const
{
   return TheDisplayInfo;
}

void ShellUI::HandleDisplayInfoChanged(const DisplayInfo& info)
{
   if((info.Number == TheDisplayInfo.Number) &&
       (info != TheDisplayInfo))
   {
      LogInfo(QString("Display info for display %1 has changed")
                 .arg(info.Number));

      TheDisplayInfo = info;
      BuildUIComponents();
   }
}


void ShellUI::HandleDisplayRemoved(const DisplayInfo& info)
{
   if(info.ID == TheDisplayInfo.ID)
   {
      TheShellWindow->close();
      TheShellWindow->deleteLater();
      TheShellWindow = nullptr;
   }
}

void ShellUI::BuildUIComponents()
{
   BuildTheShellWindow();
   BuildTheWallpaperView();
   BuildTheTaskBarView();

   TheShellWindow->show();
}

void ShellUI::BuildTheShellWindow()
{
   if(nullptr != TheShellWindow)
   {
      TheShellWindow->deleteLater();
      TheShellWindow = nullptr;
   }

   TheShellWindow = new ShellWindow();
   connect(TheShellWindow, &ShellWindow::Closed,
           this, [&](){
      emit ShellWindowClosed(GetDisplayID());
   });
}

void ShellUI::BuildTheWallpaperView()
{
   if(nullptr != TheWallpaperView)
   {
      TheWallpaperView->deleteLater();
      TheWallpaperView = nullptr;
   }

   TheWallpaperView = new Wallpaper::WallpaperView(WallpaperService,
                                                   TheShellWindow,
                                                   GetDisplayInfo());
}

void ShellUI::BuildTheTaskBarView()
{
   if(nullptr != TheTaskBarView)
   {
      TheTaskBarView->deleteLater();
      TheTaskBarView = nullptr;
   }

   TheTaskBarView = new TaskBar::TaskBarView(TaskBarService,
                                             TheShellWindow,
                                             GetDisplayInfo());
}
