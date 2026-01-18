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

void WallpaperService::HandleDisplayConfigChanged(const DisplayEvent& event,
                                                  const QMap<QString, DisplayInfo>& displays)
{
   // If there are no current workers, just add them quickly. This is at startup.
   if(Workers.isEmpty() &&
       (DisplayEvent::EventType::Added == event.Event))
   {
      for(const DisplayInfo& info : std::as_const(displays))
      {
         Workers[info.ID] = new WallpaperServiceWorker(info, &SettingsProxy, this);
      }
   }
   else
   {
      for(const WallpaperServiceWorker* worker : Workers)
      {
         // if(worker->GetDisplayInfo())
      }

   }
}

// Settings proxy now needs to send the number of the display for which the settings have changed
// then the service will notify the appropriate worker to fetch its data.
void WallpaperService::HandleSettingsChanged()
{
}
