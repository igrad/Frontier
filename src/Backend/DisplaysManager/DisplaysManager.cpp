#include "DisplaysManager.h"

#include <WindowsAPIInterface.h>

DisplaysManager::DisplaysManager(WindowsAPIInterface& windowsAPI,
                                 QObject* parent)
   : WindowsAPI(windowsAPI)
   , NumDisplays(0)
   , Displays()
   , CurrentEvent()
{
   setParent(parent);

   ConnectToWindowsAPI();
}

void DisplaysManager::RegisterMetaTypes() const
{
   qRegisterMetaType<DisplayInfo>("DisplayInfo");
   qRegisterMetaType<DisplayConfigEvent>("DisplayEvent");
}

void DisplaysManager::PollDisplaysInfo()
{
   WindowsAPI.HandlePollDisplaysInfo();
}

void DisplaysManager::HandleDisplayDetected(const DisplayInfo& info)
{
   DisplayConfigEventType eventType = DisplayConfigEventType::None;
   const auto iter = Displays.constFind(info.ID);

   if(iter == Displays.constEnd())
   {
      eventType = DisplayConfigEventType::Added;
   }
   else if(*iter != info)
   {
      eventType = DisplayConfigEventType::Changed;
   }

   CurrentEvent.Displays[info.ID] = { eventType, info };

   if(CurrentEvent.Displays.size() == NumDisplays)
   {
      FinalizeCurrentEvent();
   }
}

// This will be called before the individual DisplayInfo objects are passed in from
// the WindowsAPI.
void DisplaysManager::HandleNumberOfDisplaysChanged(uint8_t numDisplays)
{
   NumDisplays = numDisplays;
   CurrentEvent = DisplayConfigEvent();
}

void DisplaysManager::ConnectToWindowsAPI()
{
   connect(&WindowsAPI, &WindowsAPIInterface::NumberOfDisplaysChanged,
           this, &DisplaysManager::HandleNumberOfDisplaysChanged);
   connect(&WindowsAPI, &WindowsAPIInterface::DisplayDetected,
           this, &DisplaysManager::HandleDisplayDetected);
}

void DisplaysManager::FinalizeCurrentEvent()
{
   for(const auto& info : std::as_const(Displays))
   {
      // If a display existed previously but is no longer detected, add it to the
      // list of affected displays
      auto iter = CurrentEvent.Displays.find(info.ID);
      if(CurrentEvent.Displays.end() == iter)
      {
         iter->first = DisplayConfigEventType::Removed;
      }
   }

   Displays.clear();
   bool changesMade = false;
   for(const auto& info : std::as_const(CurrentEvent.Displays))
   {
      Displays[info.second.ID] = info.second;
      if(DisplayConfigEventType::None != info.first)
      {
         changesMade = true;
      }
   }

   if(changesMade)
   {
      emit DisplayConfigChanged(CurrentEvent);
   }
}
