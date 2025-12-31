#include "WallpaperSettingsProxy.h"

#include <Log.h>

#include <QColor>
#include <QVariant>

#include <iostream>

using namespace Settings;
using namespace Wallpaper;

namespace
{
   constexpr int DEFAULT_ROTATION_DURATION_MS = 30000;
   constexpr Fit DEFAULT_WALLPAPER_FIT = Fit::Fill;
   constexpr Style DEFAULT_WALLPAPER_STYLE = Style::StaticColor;
   constexpr Schedule DEFAULT_WALLPAPER_SCHEDULE = Schedule::Static;
   constexpr const char* const SETTINGS_CLIENT_NAME = "WallpaperService";
}

WallpaperSettingsProxy::WallpaperSettingsProxy()
   : SettingsClient(SETTINGS_CLIENT_NAME)
   , CurrentColors{Qt::blue}
   , CurrentDuration(DEFAULT_ROTATION_DURATION_MS)
   , CurrentFits{DEFAULT_WALLPAPER_FIT}
   , CurrentImagePaths()
   , CurrentSchedule(DEFAULT_WALLPAPER_SCHEDULE)
   , CurrentStyle(DEFAULT_WALLPAPER_STYLE)
{
   SubscribeToSettings();
}

const QList<QColor>& WallpaperSettingsProxy::GetColors() const
{
   return CurrentColors;
}

const int WallpaperSettingsProxy::GetDuration() const
{
   return CurrentDuration;
}

const QList<Fit>& WallpaperSettingsProxy::GetFits() const
{
   return CurrentFits;
}

const QStringList& WallpaperSettingsProxy::GetPaths() const
{
   return CurrentImagePaths;
}

const Schedule WallpaperSettingsProxy::GetSchedule() const
{
   return CurrentSchedule;
}

const Style WallpaperSettingsProxy::GetStyle() const
{
   return CurrentStyle;
}

void WallpaperSettingsProxy::HandleSettingWallpaperColorsChanged(const QVariant& value)
{
   std::cout << "colors changed" << std::endl;
   CurrentColors.clear();

   if(value.canConvert<QStringList>())
   {
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

   QStringList strList;
   for(const QColor& color : std::as_const(CurrentColors))
   {
      strList.push_back(color.name());
   }

   LogInfo(QString("Wallpaper CurrentColors changed to \"%1\"").arg(strList.join(",")));

   emit SettingsChanged();
}

void WallpaperSettingsProxy::HandleSettingWallpaperDurationChanged(const QVariant& value)
{
   std::cout << "Duration changed" << std::endl;
   if(value.canConvert<int>())
   {
      CurrentDuration = value.toInt();
      LogInfo(QString("Wallpaper rotation duration changed to %1msec").arg(CurrentDuration));

      emit SettingsChanged();
   }
}

void WallpaperSettingsProxy::HandleSettingWallpaperFitsChanged(const QVariant& value)
{
   std::cout << "fits changed" << std::endl;
   const QVariantList list = value.toList();

   bool canConvert = true;
   QList<Fit> fits;
   QStringList strList;
   for(const QVariant& val : std::as_const(list))
   {
      if(!val.canConvert<Fit>())
      {
         canConvert = false;
         break;
      }

      const Fit fit = val.value<Fit>();
      fits.push_back(fit);
      strList.push_back(ToString(fit));
   }

   if(canConvert)
   {
      if(fits != CurrentFits)
      {
         CurrentFits = fits;

         LogInfo(QString("Wallpaper CurrentFits changed to \"%1\"").arg(strList.join(",")));

         emit SettingsChanged();
      }
   }
}

void WallpaperSettingsProxy::HandleSettingWallpaperImagePathsChanged(const QVariant& value)
{
   std::cout << "imagePaths changed" << std::endl;
   CurrentImagePaths.clear();

   // NOTE: Is this necessary?
   // if(value.canConvert<QStringList>())
   // {
   //    CurrentImagePaths = value.toStringList();
   // }
   // else	// Just pass in a single image
   // {
   CurrentImagePaths = value.toStringList();
   // }

   LogInfo(QString("Wallpaper CurrentImagePaths changed to \"%1\"")
              .arg(CurrentImagePaths.join(",")));
   emit SettingsChanged();
}

void WallpaperSettingsProxy::HandleSettingWallpaperScheduleChanged(const QVariant& value)
{
   std::cout << "schedule changed" << std::endl;
   const Schedule newSchedule = value.value<Schedule>();

   if(newSchedule != CurrentSchedule)
   {
      CurrentSchedule = newSchedule;
      LogInfo(QString("Wallpaper schedule changed to: %1").arg(ToString(CurrentSchedule)));

      emit SettingsChanged();
   }
}

// TODO: Future support for different image styles in the rotation
void WallpaperSettingsProxy::HandleSettingWallpaperStyleChanged(const QVariant& value)
{
   std::cout << "style changed" << std::endl;
   if(value.canConvert<Style>())
   {
      const Style style = value.value<Style>();

      if(style != CurrentStyle)
      {
         CurrentStyle = style;

         LogInfo(QString("Wallpaper style changed to %1").arg(ToString(style)));

         emit SettingsChanged();
      }
   }
}

void WallpaperSettingsProxy::SubscribeToSettings()
{
   SettingsClient.SubscribeToSetting(Setting::WallpaperColors, this);
   SettingsClient.SubscribeToSetting(Setting::WallpaperDuration, this);
   SettingsClient.SubscribeToSetting(Setting::WallpaperFits, this);
   SettingsClient.SubscribeToSetting(Setting::WallpaperImagePaths, this);
   SettingsClient.SubscribeToSetting(Setting::WallpaperSchedule, this);
   SettingsClient.SubscribeToSetting(Setting::WallpaperStyle, this);
}
