#include "WallpaperService.h"

#include <Log.h>

#include <Settings.h>

#include <QVariant>

using namespace Settings;
using namespace Wallpaper;

WallpaperService::WallpaperService()
   : Settings("WallpaperService")
   , Schedule(WallpaperSchedule::None)
{
}

WallpaperService::~WallpaperService() = default;

void WallpaperService::HandleSettingWallpaperScheduleChanged(const QVariant& value)
{
   const WallpaperSchedule newSchedule = value.value<WallpaperSchedule>();

   if(newSchedule != Schedule)
   {
      Schedule = newSchedule;
      LogInfo("Wallpaper schedule changed to: ")
   }
}

void WallpaperService::SubscribeToSettings()
{
   Settings.SubscribeToSetting(Setting::WallpaperSchedule, this);
}
