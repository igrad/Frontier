#include "ShellUI.h"
#include "UIManager.h"

#include <AssetManager.h>
#include <Wallpaper/WallpaperView.h>
#include <WindowsAPI/WindowsAPI.h>

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
   , DisplaysInfoRequested(false)
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
   LogInfo("Displays info received");
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
   LogInfo("UIManager received TaskBar service");
   TaskBarService = XPtr(service);
   RequestDisplaysInfo();
}

void UIManager::HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service)
{
   LogInfo("UIManager received wallpaper service");
   WallpaperService = XPtr(service);
   RequestDisplaysInfo();
}

void UIManager::Start()
{
   emit UIConnectedToServiceComponents();
}

void UIManager::BuildShellWindows()
{
   LogInfo("Building shell windows");

   if(DisplaysInfo.count() < Shells.count())
   {
      LogInfo(QString("Monitor count has dropped from %1 to %2")
                 .arg(Shells.count(), DisplaysInfo.count()));

      while(DisplaysInfo.count() < Shells.count())
      {
         ShellUI* ui = Shells.last();
         LogInfo(QString("Discarding existing ShellUI for monitor %1")
          .arg(ui->GetDisplayNumber()));
         ui = nullptr;
         Shells.removeLast();
      }
   }

   for(const DisplayInfo& info : std::as_const(DisplaysInfo))
   {
      const uint8_t id = info.Number;

      auto shellIter = std::find_if(Shells.begin(),
                                    Shells.end(),
                                    [&](const ShellUI* shell) {
         return id == shell->GetDisplayNumber();
      });

      const bool exists = (Shells.end() != shellIter);
      if(exists &&
          (info != (*shellIter)->GetDisplayInfo()))
      {
         LogInfo(QString("DisplayInfo changed for display: %1")
                    .arg(info.Number));
         (*shellIter)->HandleDisplayInfoUpdated(info);
      }

      if(!exists)
      {
         LogInfo(QString("Building ShellUI for display: %1")
                    .arg(info.Number));
         Shells.push_back(new ShellUI(TaskBarService,
                                      WallpaperService,
                                      info,
                                      this));
      }
   }
}

void UIManager::RequestDisplaysInfo()
{
   if(!DisplaysInfoReceived &&
       !TaskBarService.isNull() &&
       !WallpaperService.isNull() &&
       (nullptr != TheAssetManager))
   {
      if(!DisplaysInfoRequested)
      {
         LogInfo("Requesting initial display info");
         emit PollDisplaysInfo();
         DisplaysInfoRequested = true;
         // Note: Should we yield the thread right here?
      }
      if(!DisplaysInfoReceived)
      LogInfo("1")
      if(TaskBarService.isNull())
      LogInfo("2")
      if(WallpaperService.isNull())
      LogInfo("3")
      if(nullptr == TheAssetManager)
      LogInfo("4")
   }
}
