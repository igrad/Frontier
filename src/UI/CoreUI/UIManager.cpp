#include "ShellUI.h"
#include "UIManager.h"

#include <AssetManager.h>
#include <Wallpaper/WallpaperView.h>

#include <QMetaObject>

UIManager::UIManager(DataAccessThreadManager* dataAccess,
                     BackendThreadManager* backend)
   : DataAccess(dataAccess)
   , Backend(backend)
   , TheAssetLoader(nullptr)
   , TaskBarService(nullptr)
   , WallpaperService(nullptr)
   , TheAssetManager(nullptr)
   , Shells()
   , DisplaysInfo()
   , DisplaysInfoReceived(false)
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
}

void UIManager::HandleDisplaysInfo(const QList<DisplayInfo>& info)
{
   DisplaysInfoReceived = true;

   if(info != DisplaysInfo)
   {
      DisplaysInfo = info;
      BuildShellWindows();
   }
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

void UIManager::HandlePassAssetLoader(Assets::AssetLoaderInterface* loader)
{
   TheAssetLoader = XPtr(loader);
   TheAssetManager = new Assets::AssetManager(TheAssetLoader, this);
   // Don't bother trying to build UI components until service thread components are built
}

void UIManager::HandlePassTaskBarService(TaskBar::TaskBarServiceInterface* service)
{
   TaskBarService = XPtr(service);
   BuildShellWindows();
}

void UIManager::HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service)
{
   WallpaperService = XPtr(service);
   BuildShellWindows();
}

void UIManager::Start()
{
   emit UIConnectedToServiceComponents();
}

void UIManager::BuildShellWindows()
{
   if(!DisplaysInfoReceived ||
       TaskBarService.isNull() ||
       WallpaperService.isNull() ||
       (nullptr == TheAssetManager))
   {
      return;
   }

   LogInfo("Building shell windows");

   if(DisplaysInfo.count() < Shells.count())
   {
      LogInfo(QString("Monitor count has dropped from %1 to %2")
                 .arg(Shells.count(), DisplaysInfo.count()));

      while(DisplaysInfo.count() < Shells.count())
      {
         ShellUI* ui = Shells.last();
         LogInfo(QString("Discarding existing ShellUI for monitor %1")
          .arg(ui->GetDisplayID()));
         ui = nullptr;
         Shells.removeLast();
      }
   }

   for(const DisplayInfo& info : std::as_const(DisplaysInfo))
   {
      const uint8_t id = info.ID;

      auto shellIter = std::find_if(Shells.begin(),
                                    Shells.end(),
                                    [&](const ShellUI* shell) {
         return id == shell->GetDisplayID();
      });

      const bool exists = (Shells.end() != shellIter);
      if(exists &&
          (info != (*shellIter)->GetDisplayInfo()))
      {
         LogInfo(QString("DisplayInfo changed for display: %1")
                    .arg(info.ID));
         (*shellIter)->HandleDisplayInfoUpdated(info);
      }

      if(!exists)
      {
         LogInfo(QString("Building ShellUI for display: %1")
                    .arg(info.ID));
         Shells.push_back(new ShellUI(TaskBarService,
                                      WallpaperService,
                                      info,
                                      this));
      }
   }
}
