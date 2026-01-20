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

// TheAssetLoader does not need to be deleted manually - it's parent to this and
// will auto-delete
// UIManager::~UIManager()
// {
// }

void UIManager::HandleDisplayConfigChanged(const DisplayConfigEvent& event)
{
   DisplaysInfoReceived = true;

   for(const QPair<DisplayConfigEventType, DisplayInfo>& info : std::as_const(event.Displays))
   {
      switch(info.first)
      {
      case DisplayConfigEventType::Added:
         BuildShellWindow(info.second);
         break;
      case DisplayConfigEventType::Removed:
         RemoveShellWindow(info.second);
         break;
      case DisplayConfigEventType::Changed:
         Shells[info.second.ID]->HandleDisplayInfoChanged(info.second);
         break;
      case DisplayConfigEventType::None:
         // This display has not changed at all - do nothing
         break;
      }
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

void UIManager::BuildShellWindow(const DisplayInfo& info)
{
   if(!Shells.contains(info.ID))
   {
      Shells[info.ID] = new ShellUI(TaskBarService,
                                    WallpaperService,
                                    info,
                                    this);
   }
}

void UIManager::RemoveShellWindow(const DisplayInfo& info)
{
   auto iter = Shells.find(info.ID);

   if(Shells.end() != iter)
   {
      iter.value()->HandleDisplayRemoved(info);
      iter.value()->deleteLater();
      Shells[info.ID] = nullptr;
      Shells.erase(iter);
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
