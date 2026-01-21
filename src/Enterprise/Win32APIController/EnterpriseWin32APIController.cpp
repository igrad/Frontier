#include "EnterpriseWin32APIController.h"

#include <Win32APIWrapper/Win32APIWrapperFake.h>
#include <Log.h>

#include <string>

using namespace Enterprise;
typedef Win32APIWrapperFake API;
EnterpriseWin32APIController::EnterpriseWin32APIController(QObject* parent)
   : QObject(parent)
{

}

void EnterpriseWin32APIController::HandleDisplayInfoModified(const DisplayConfigEvent& event)
{
   LogInfo("EnterpriseWin32APIController received display config event");
   API::DisplayMonitors.clear();
   API::MonitorInfos.clear();
   API::DisplayDevices.clear();
   API::DPIs.clear();

   for(const QPair<DisplayConfigEventType, DisplayInfo>& pair : event.Displays)
   {
      switch(pair.first)
      {
      case DisplayConfigEventType::Added:
      case DisplayConfigEventType::Changed:
         AddDisplay(pair.second);
         break;
      default:
         // No
         break;
      }
   }
}

// This func was done while very tired and needs to be reviewed
void EnterpriseWin32APIController::AddDisplay(const DisplayInfo& info)
{
   // Convert from Enterprise using 1-index display numbers to Frontier's 0-indexing
   // Wahoo
   int num = info.Number - 1;

   // DisplayMonitors
   API::DisplayMonitor monitor;
   monitor.hMonitor = *info.Handle;
   LPRECT prect = new tagRECT();
   tagRECT rect = *prect;
   QRect qrect = info.Rect;
   rect.bottom = qrect.bottom();
   rect.left = qrect.left();
   rect.top = qrect.top();
   rect.right = qrect.right();
   monitor.lproClip = prect;
   API::DisplayMonitors[num] = monitor;

   // MonitorInfos
   MONITORINFOEX mie;
   mie.rcMonitor = rect;
   mie.rcWork = rect;
   wcscpy(mie.szDevice, info.DisplayName.toStdWString().c_str());
   if(info.IsPrimary)
   {
      mie.dwFlags |= MONITORINFOF_PRIMARY;
   }
   API::MonitorInfos[num] = mie;

   // DisplayDevices
   API::DisplayDevice dev;
   dev.DeviceID = info.ID.toStdString().c_str();
   dev.MonitorName = info.DisplayName.toStdString().c_str();
   dev.DeviceName = info.DisplayName.toStdString().c_str();
   API::DisplayDevices[num] = dev;

   // DPIs
   API::DPIs[num] = {info.XDPI, info.YDPI};
}
