#include "WallpaperService.h"

using namespace Wallpaper;

WallpaperService::WallpaperService()
   : Settings("WallpaperService")
{
}

WallpaperService::~WallpaperService() = default;

void WallpaperService::HandleSettingWallpaperScheduleChanged(const QVariant& value)
{
   Q_UNUSED(value)
}

void WallpaperService::SubscribeToSettings()
{
   Settings.SubscribeToSetting(Setting::WallpaperSchedule, this);
}
