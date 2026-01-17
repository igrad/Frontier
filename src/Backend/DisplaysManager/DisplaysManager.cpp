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

void DisplaysManager::HandleDisplayDetected(const DisplayInfo& info)
{
   DisplayInfo i = info;
   i.Number = CurrentEventInfo.size();
   CurrentEventInfo << i;

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
   CurrentEventInfo.reserve(NumDisplays);
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
   for(DisplayInfo info : CurrentEventInfo)
   {
      // Check if this display is already known
      auto ptr = std::find_if(Displays.cbegin(),
                               Displays.cend(),
                               [&](const DisplayInfo& display){
                                  return display.Handle == info.Handle;
                               });

      if(Displays.cend() == ptr)
      {
         // This display isn't known
         CurrentEvent.AffectedDisplays.insert(ptr->Number);
      }
      else if(*ptr != info)
      {
         // This display is known and has changed
         CurrentEvent.AffectedDisplays.insert(ptr->Number);
      }
   }

   Displays = CurrentEventInfo;

   if(!CurrentEvent.AffectedDisplays.isEmpty())
   {
      emit DisplayConfigChanged(CurrentEvent, CurrentEventInfo);
   }
}
