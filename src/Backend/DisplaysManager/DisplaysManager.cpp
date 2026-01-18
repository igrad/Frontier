#include "DisplaysManager.h"

#include <WindowsAPIInterface.h>

DisplaysManager::DisplaysManager(WindowsAPIInterface& windowsAPI,
                                 QObject* parent)
   : WindowsAPI(windowsAPI)
   , NumDisplays(0)
   , Displays()
   , CurrentEvent()
   , CurrentEventInfo()
{
   setParent(parent);

   ConnectToWindowsAPI();
}

void DisplaysManager::RegisterMetaTypes() const
{
   qRegisterMetaType<DisplayEvent>("DisplayEvent");
   qRegisterMetaType<DisplayInfo>("DisplayInfo");
}

void DisplaysManager::PollDisplaysInfo()
{
   WindowsAPI.HandlePollDisplaysInfo();
}

void DisplaysManager::HandleDisplayDetected(const DisplayInfo& info)
{
   CurrentEventInfo[info.ID] = info;

   if(CurrentEventInfo.size() == NumDisplays)
   {
      FinalizeCurrentEvent();
   }
}

// This will be called before the individual DisplayInfo objects are passed in from
// the WindowsAPI.
void DisplaysManager::HandleNumberOfDisplaysChanged(uint8_t numDisplays)
{
   DisplayEvent event;
   if(numDisplays > NumDisplays)
   {
      event.Event = DisplayEvent::EventType::Added;
   }
   else if(numDisplays < NumDisplays)
   {
      event.Event = DisplayEvent::EventType::Removed;
   }

   NumDisplays = numDisplays;
   CurrentEvent = event;
   CurrentEventInfo.clear();
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
   for(const auto& info : std::as_const(CurrentEventInfo))
   {
      const auto iter = Displays.constFind(info.ID);
      const bool exists = (Displays.cend() != iter);

      if(!exists || (exists && (*iter != info)))
      {
         CurrentEvent.AffectedDisplays.insert(info.ID);
      }
   }

   if(DisplayEvent::EventType::Removed == CurrentEvent.Event)
   {
      for(const auto& info : std::as_const(Displays))
      {
         // If a display existed previously but is no longer detected, add it to the
         // list of affected displays
         if(!CurrentEventInfo.contains(info.ID))
         {
            CurrentEvent.AffectedDisplays.insert(info.ID);
         }
      }
   }

   Displays = CurrentEventInfo;

   if(!CurrentEvent.AffectedDisplays.isEmpty())
   {
      emit DisplayConfigChanged(CurrentEvent, CurrentEventInfo);
   }
}
