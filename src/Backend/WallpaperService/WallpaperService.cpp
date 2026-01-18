#include "WallpaperService.h"

#include <Log.h>

#include <SettingsService/Settings.h>

#include <QVariant>

using namespace Settings;
using namespace Wallpaper;

namespace
{
   constexpr int DEFAULT_ROTATION_DURATION_MS = 30000;
}

WallpaperService::WallpaperService(QObject* parent)
   : SettingsProxy()
   , Workers()
{
   setParent(parent);

   connect(&SettingsProxy, &WallpaperSettingsProxy::SettingsChanged,
           this, &WallpaperService::HandleSettingsChanged);
}

void WallpaperService::RegisterMetaTypes() const
{
   qRegisterMetaType<Wallpaper::ViewData>("Wallpaper::ViewData");
}

void WallpaperService::HandleDisplayConfigChanged(const DisplayConfigEvent& event)
{
   // If there are no current workers, just add them quickly. This is at startup.
   if(Workers.isEmpty())
   {
      for(const QPair<DisplayConfigEventType, DisplayInfo>& info : std::as_const(event.Displays))
      {
         Workers[info.second.ID] = new WallpaperServiceWorker(info.second,
                                                              &SettingsProxy,
                                                              this);
      }
   }
   else
   {
      for(const QPair<DisplayConfigEventType, DisplayInfo>& info : std::as_const(event.Displays))
      {
         switch(info.first)
         {
         case DisplayConfigEventType::Added:
            Workers[info.second.ID] = new WallpaperServiceWorker(info.second,
                                                                 &SettingsProxy,
                                                                 this);
            break;
         case DisplayConfigEventType::Changed:
            Workers[info.second.ID]->HandleDisplayConfigChanged(info.second);
            break;
         case DisplayConfigEventType::Removed:
            Workers[info.second.ID]->HandleDisplayRemoved(info.second);
            break;
         default:
            LogWarn(QString("Received None event for display ID: %1")
                       .arg(info.second.ID));
            break;
         }
      }
   }
}

// Settings proxy now needs to send the number of the display for which the settings have changed
// then the service will notify the appropriate worker to fetch its data.
void WallpaperService::HandleSettingsChanged()
{
}
