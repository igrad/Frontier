#include "ShellUI.h"
#include "UIManager.h"

#include <AssetManager.h>
#include <DisplaysManagerInterface.h>
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
   , Displays()
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

void UIManager::HandleDisplayConfigChanged(const DisplayConfigEvent& event)
{
   LogInfo("Displays info received");
   DisplaysInfoReceived = true;

   QMap<DisplayID, DisplayInfo> displays;
   for(const QPair<DisplayConfigEventType, DisplayInfo>& info : std::as_const(event.Displays))
   {
      displays[info.second.ID] = info.second;
   }

   // Note: Maybe need to do more here?
   if(displays != Displays)
   {
      Displays = displays;
      BuildShellWindows();
   }
}

void UIManager::HandleDataAccessThreadStarted()
{
   connect(DataAccess.get(), &DataAccessThreadManager::PassAssetLoader,
           this, &UIManager::HandlePassAssetLoader);
   QMetaObject::invokeMethod(DataAccess.Object,
                             "HandleRequestAssetLoader");
}

void UIManager::HandleServiceThreadStarted()
{
   connect(this, &UIManager::PollDisplaysInfo,
           Backend.get(), &BackendThreadManager::HandlePollDisplaysInfo);

   connect(Backend.get(), &BackendThreadManager::PassDisplaysManager,
           this, &UIManager::HandlePassDisplaysManager);
   QMetaObject::invokeMethod(Backend.Object,
                             "HandleRequestPassDisplaysManager");

   connect(Backend.get(), &BackendThreadManager::PassTaskBarService,
           this, &UIManager::HandlePassTaskBarService);
   QMetaObject::invokeMethod(Backend.Object,
                             "HandleRequestPassTaskBarService");

   connect(Backend.get(), &BackendThreadManager::PassWallpaperService,
           this, &UIManager::HandlePassWallpaperService);
   QMetaObject::invokeMethod(Backend.Object,
                             "HandleRequestPassWallpaperService");
}

void UIManager::HandlePassDisplaysManager(DisplaysManagerInterface* manager)
{
   DisplaysManager = XPtr(manager);
   connect(DisplaysManager.get(), &DisplaysManagerInterface::DisplayConfigChanged,
           this, &UIManager::HandleDisplayConfigChanged);
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
   RequestDisplaysInfo();
}

void UIManager::HandlePassWallpaperService(Wallpaper::WallpaperServiceInterface* service)
{
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

   if(Displays.count() < Shells.count())
   {
      LogInfo(QString("Monitor count has dropped from %1 to %2")
                 .arg(Shells.count(), Displays.count()));

      while(Displays.count() < Shells.count())
      {
         ShellUI* ui = Shells.last();
         LogInfo(QString("Discarding existing ShellUI for monitor %1")
          .arg(ui->GetDisplayID()));
         ui = nullptr;
         Shells.removeLast();
      }
   }

   for(const DisplayInfo& info : std::as_const(Displays))
   {
      const DisplayID id = info.ID;

      auto shellIter = std::find_if(Shells.begin(),
                                    Shells.end(),
                                    [&](const ShellUI* shell) {
         return (id == shell->GetDisplayID());
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
       (nullptr != TheAssetManager) &&
       !DisplaysInfoRequested)
   {
      LogInfo("Requesting initial display info");
      emit PollDisplaysInfo();
      DisplaysInfoRequested = true;
      // Note: Should we yield the thread right here?
   }
}
