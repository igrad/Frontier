#include "WallpaperService.h"

#include <Log.h>

#include <SettingsService/Settings.h>

#include <QVariant>

using namespace Settings;
using namespace Wallpaper;

namespace
{
   constexpr Style DEFAULT_WALLPAPER_STYLE = Style::StaticColor;
   constexpr Fit DEFAULT_WALLPAPER_FIT = Fit::Fill;
   constexpr int DEFAULT_ROTATION_DURATION_MS = 30000;
}

WallpaperService::WallpaperService(QObject* parent)
   : QObject(parent)
   , Settings("WallpaperService")
   , CurrentSchedule(Schedule::None)
   , CurrentImagePaths()
   , CurrentColors{QColor("blue")}
   , CurrentColorsIndex(0)
   , CurrentDuration(DEFAULT_ROTATION_DURATION_MS)
   , CurrentStyle(DEFAULT_WALLPAPER_STYLE)
   , RotationTimer(this)
   , ShuffleRando()
{
   RegisterMetaTypes();
   SubscribeToSettings();

   RotationTimer.setTimerType(Qt::TimerType::CoarseTimer);
   RotationTimer.setInterval(DEFAULT_ROTATION_DURATION_MS);
}

void WallpaperService::RegisterMetaTypes() const
{
   qRegisterMetaType<ViewData>("Wallpaper::ViewData");
}

void WallpaperService::HandleSettingWallpaperColorsChanged(const QVariant& value)
{
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

   CalculateCurrentWallpaperData();
}

void WallpaperService::HandleSettingWallpaperDurationChanged(const QVariant& value)
{
   if(value.canConvert<int>())
   {
      CurrentDuration = value.toInt();
      RotationTimer.setInterval(CurrentDuration);
      LogInfo(QString("Wallpaper rotation duration changed to %1msec").arg(CurrentDuration));

      CalculateCurrentWallpaperData();
   }
}

void WallpaperService::HandleSettingWallpaperFitChanged(const QVariant& value)
{
   if(value.canConvert<Fit>())
   {
      Fit fit = value.value<Fit>();

      if(fit != CurrentFit)
      {
         if(Fit::None == fit)
         {
            fit = DEFAULT_WALLPAPER_FIT;
            LogWarn("Invalid wallpaper fit was parsed. Defaulting.");
         }

         CurrentFit = fit;

         LogInfo(QString("Wallpaper CurrentFit changed to %1").arg(ToString(CurrentFit)));
         CalculateCurrentWallpaperData();
      }
   }
}

void WallpaperService::HandleSettingWallpaperImagePathsChanged(const QVariant& value)
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

   LogInfo(QString("Wallpaper CurrentImagePaths changed to \"%1\"")
              .arg(CurrentImagePaths.join(",")));

   CalculateCurrentWallpaperData();
}

void WallpaperService::HandleSettingWallpaperScheduleChanged(const QVariant& value)
{
   const Schedule newSchedule = value.value<Schedule>();

   if(newSchedule != CurrentSchedule)
   {
      CurrentSchedule = newSchedule;
      LogInfo(QString("Wallpaper schedule changed to: %1").arg(ToString(CurrentSchedule)));

      CalculateCurrentWallpaperData();
   }
}

// TODO: Future support for different image styles in the rotation
void WallpaperService::HandleSettingWallpaperStyleChanged(const QVariant& value)
{
   if(value.canConvert<Style>())
   {
      const Style style = value.value<Style>();

      if(style != CurrentStyle)
      {
         CurrentStyle = style;

         LogInfo(QString("Wallpaper style changed to %1").arg(ToString(style)));
         CalculateCurrentWallpaperData();
      }
   }
}

void WallpaperService::HandleRotationTimeout()
{
   LogInfo("Wallpaper rotation timer is triggering a wallpaper change");
   CalculateCurrentWallpaperData(true);
}

void WallpaperService::SubscribeToSettings()
{
   Settings.SubscribeToSetting(Setting::WallpaperColors, this);
   Settings.SubscribeToSetting(Setting::WallpaperDuration, this);
   Settings.SubscribeToSetting(Setting::WallpaperFit, this);
   Settings.SubscribeToSetting(Setting::WallpaperImagePaths, this);
   Settings.SubscribeToSetting(Setting::WallpaperSchedule, this);
   Settings.SubscribeToSetting(Setting::WallpaperStyle, this);
}

void WallpaperService::CalculateCurrentWallpaperData(bool triggeredByRotationTimer)
{
   ViewData data;
   data.Fit = Fit::Fill;

   switch(CurrentSchedule)
   {
   case Schedule::Sequence:
   {
      RotationTimer.start(CurrentDuration);
      CalculateNextColor(false);
      break;
   }
   case Schedule::Shuffle:
   {
      RotationTimer.start(CurrentDuration);
      if((Style::StaticColor == CurrentStyle) ||
          (Style::DynamicColor == CurrentStyle))
      {
         CalculateNextColor(true);
      }
      break;
   }
   default:
   {
      break;
   }
   }
}

void WallpaperService::CalculateNextColor(bool shuffled)
{
   int index = CurrentColorsIndex;

   if(shuffled)
   {
      int tries = 5;
      do
      {
         index = ShuffleRando.Index(CurrentColors);
         --tries;
      }
      while((tries > 0) &&
               (index == CurrentColorsIndex) &&
               (CurrentColors.size() > 1));
   }
   else
   {
      ++index;
      if(index >= CurrentColors.count())
      {
         index = 0;
      }
   }

   CurrentColorsIndex = index;
}
