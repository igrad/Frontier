#include "WindowsEventMessageFilter.h"

#include <windows.h>
#include <winuser.h>
#include <dbt.h>

WindowsEventMessageFilter::WindowsEventMessageFilter(QObject* parent)
{
   setParent(parent);
}

bool WindowsEventMessageFilter::nativeEventFilter(const QByteArray& eventType,
                                                  void* message,
                                                  qintptr*)
{
   // Return true if we no longer want to handle the received message type
   bool retVal = true;

   if(("windows_generic_MSG" == eventType) ||
       ("windows_HWND_MSG" == eventType))
   {
      retVal = HandleGenericMessage(message);
   }
   else if("windows_dispatcher_MSG" == eventType)
   {
      // const MSG* const msg = static_cast<MSG*>(message);
      // TODO: Handle hotkeys
   }

   return retVal;
}

bool WindowsEventMessageFilter::HandleGenericMessage(void* message)
{
   const MSG* const msg = static_cast<MSG*>(message);
   if(WM_DEVICECHANGE == msg->message)
   {
      return HandleDeviceChange(msg);
   }

   return false;
}

bool WindowsEventMessageFilter::HandleDeviceChange(const MSG* message)
{
   switch(message->wParam)
   {
   case DBT_DEVICEARRIVAL:
   case DBT_DEVICEREMOVECOMPLETE:
   case DBT_CONFIGCHANGED:
   case DBT_DEVNODES_CHANGED:
      emit DevicesChanged();
      break;
   default:
      return true;
   }

   return false;
}
