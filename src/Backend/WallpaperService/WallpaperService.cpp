#include "WallpaperService.h"

#include <Log.h>

#include <Settings.h>

#include <QVariant>

using namespace Settings;
using namespace Wallpaper;

namespace
{
   constexpr WallpaperMode DEFAULT_WALLPAPER_MODE = WallpaperMode::StaticImage;
}

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
      ImagePaths = value.toStringList();
   }
   else
   {
      ImagePaths.push_back(value.toString());
   }
}

void WallpaperService::HandleSettingWallpaperColors(const QVariant& value)
{
   if(value.canConvert<QStringList>())
   {
      const QStringList strList = value.toStringList();
      for(const QString& str : strList)
      {
         Colors.push_back(QColor(str));
      }
   }
   else
   {
      Colors.push_back(QColor(value.toString()));
   }
}

void WallpaperService::HandleSettingWallpaperDuration(const QVariant& value)
{
   if(value.canConvert<int>())
   {
      Duration = value.toInt();
   }
}

void WallpaperService::HandleSettingWallpaperActiveMode(const QVariant& value)
{
   WallpaperMode mode = StringToEnum<WallpaperMode>(value.toString());

   if(WallpaperMode::None == mode)
   {
      mode = DEFAULT_WALLPAPER_MODE;
   }

   Mode = mode;
}

void WallpaperService::SubscribeToSettings()
{
   Settings.SubscribeToSetting(Setting::WallpaperSchedule, this);
}
