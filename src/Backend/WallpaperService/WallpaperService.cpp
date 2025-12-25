#include "WallpaperService.h"

#include <Log.h>

#include <Settings.h>

#include <QVariant>

using namespace Settings;
using namespace Wallpaper;

namespace
{
   constexpr Mode DEFAULT_WALLPAPER_MODE = Mode::StaticImage;
}

WallpaperService::WallpaperService(QObject* parent)
   : QObject(parent)
   , Settings("WallpaperService")
   , CurrentSchedule(Schedule::None)
   , CurrentImagePaths()
   , CurrentColors{QColor("blue")}
   , CurrentDuration(0)
   , CurrentMode(Mode::StaticColor)
{
   RegisterMetaTypes();
   SubscribeToSettings();
}

WallpaperService::~WallpaperService() = default;

void WallpaperService::HandleSettingWallpaperScheduleChanged(const QVariant& value)
{
   const Schedule newSchedule = value.value<Schedule>();

   if(newSchedule != CurrentSchedule)
   {
      CurrentSchedule = newSchedule;
      LogInfo("Wallpaper schedule changed to: ")
   }
}

void WallpaperService::HandleSettingWallpaperImagePaths(const QVariant& value)
{
   CurrentImagePaths.clear();

   if(value.canConvert<QStringList>())
   {
      CurrentImagePaths = value.toStringList();
   }
   else
   {
      CurrentImagePaths.push_back(value.toString());
   }
}

void WallpaperService::HandleSettingWallpaperColors(const QVariant& value)
{
   if(value.canConvert<QStringList>())
   {
      CurrentColors.clear();

      const QStringList strList = value.toStringList();
      for(const QString& str : strList)
      {
         CurrentColors.push_back(QColor(str));
      }
   }
   else
   {
      CurrentColors.push_back(QColor(value.toString()));
   }
}

void WallpaperService::HandleSettingWallpaperDuration(const QVariant& value)
{
   if(value.canConvert<int>())
   {
      CurrentDuration = value.toInt();
   }
}

void WallpaperService::HandleSettingWallpaperActiveMode(const QVariant& value)
{
   Mode mode = StringToEnum<Mode>(value.toString());

   if(Mode::None == mode)
   {
      mode = DEFAULT_WALLPAPER_MODE;
   }

   CurrentMode = mode;
}

void WallpaperService::RegisterMetaTypes() const
{
   qRegisterMetaType<WallpaperData>("Wallpaper::WallpaperData");
}

void WallpaperService::SubscribeToSettings()
{
   Settings.SubscribeToSetting(Setting::WallpaperSchedule, this);
}
