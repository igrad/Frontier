#include "ShellWindow.h"
#include "UIManager.h"

#include <AssetManager.h>
#include <Wallpaper/WallpaperView.h>

#include <QMetaObject>

UIManager::UIManager(DataAccessThreadManager* dataAccess,
                     BackendThreadManager* backend)
   : DataAccess(dataAccess)
   , Backend(backend)
   , TheShellWindow(nullptr)
   , TheAssetLoader(nullptr)
   , TheAssetManager(nullptr)
   , TaskBarService(nullptr)
   , WallpaperService(nullptr)
   , TheWallpaperView(nullptr)
{
   connect(this, &UIManager::UIConnectedToServiceComponents,
           DataAccess.get(), &DataAccessThreadManager::HandleUIConnectedToComponents);

   connect(DataAccess.get(), &DataAccessThreadManager::DataAccessThreadStarted,
           this, &UIManager::HandleDataAccessThreadStarted);
   connect(Backend.get(), &BackendThreadManager::ServiceThreadStarted,
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

void UIManager::HandleDataAccessThreadStarted()
{
   LogInfo("Handling DataAccess thread started");

   connect(DataAccess.get(), &DataAccessThreadManager::PassAssetLoader,
           this, &UIManager::HandlePassAssetLoader);
   QMetaObject::invokeMethod(DataAccess.Object,
                             "HandleRequestAssetLoader");
}

void UIManager::HandleServiceThreadStarted()
{
   LogInfo("Handling Service thread started");

   connect(Backend.get(), &BackendThreadManager::PassTaskBarService,
           this, &UIManager::HandlePassTaskBarService);
   QMetaObject::invokeMethod(Backend.Object,
                             "HandleRequestPassTaskBarService");

   connect(Backend.get(), &BackendThreadManager::PassWallpaperService,
           this, &UIManager::HandlePassWallpaperService);
   QMetaObject::invokeMethod(Backend.Object,
                             "HandleRequestPassWallpaperService");
}

void UIManager::HandlePassAssetLoader(AssetLoaderInterface* loader)
{
   TheAssetLoader = XPtr(loader);
   TheAssetManager = new AssetManager(TheAssetLoader, this);
   // Don't bother trying to build UI components until service thread components are built
}

void UIManager::HandlePassTaskBarService(TaskBar::TaskBarServiceInterface* service)
{
   TaskBarService = XPtr(service);
   BuildUIComponents();
}

void UIManager::HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service)
{
   WallpaperService = XPtr(service);
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
