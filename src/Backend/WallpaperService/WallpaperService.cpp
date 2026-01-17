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

   Workers.reserve(2);

   connect(&SettingsProxy, &WallpaperSettingsProxy::SettingsChanged,
           this, &WallpaperService::HandleSettingsChanged);
}

void WallpaperService::RegisterMetaTypes() const
{
   qRegisterMetaType<ViewData>("Wallpaper::ViewData");
}

void WallpaperService::HandleDisplayConfigChanged(const DisplayEvent& event,
                                                  const QSet<DisplayInfo>& displays)
{
}

// Settings proxy now needs to send the number of the display for which the settings have changed
// then the service will notify the appropriate worker to fetch its data.
void WallpaperService::HandleSettingsChanged()
{
}
