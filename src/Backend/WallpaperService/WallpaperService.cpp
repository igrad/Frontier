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
   SubscribeToSettings();
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

void WallpaperService::HandleSettingWallpaperImagePaths(const QVariant& value)
{
   if(value.canConvert<QStringList>())
   {
      // It's a list
      ImagePaths = value.toStringList();
   }
   else
   {
      ImagePaths.push_back(value.toString());
   }
}

void WallpaperService::HandleSettingWallpaperColor(const QVariant& value)
{

}

void WallpaperService::HandleSettingWallpaperDuration(const QVariant& value)
{

}

void WallpaperService::HandleSettingWallpaperActiveMode(const QVariant& value)
{

}

void WallpaperService::SubscribeToSettings()
{
   Settings.SubscribeToSetting(Setting::WallpaperSchedule, this);
}
