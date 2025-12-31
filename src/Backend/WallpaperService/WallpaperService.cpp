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
   : QObject(parent)
   , SettingsProxy()
   , CurrentColorsIndex(0)
   , CurrentImageIndex(0)
   , RotationTimer(this)
   , ShuffleRando()
{
   RegisterMetaTypes();

   RotationTimer.setTimerType(Qt::TimerType::CoarseTimer);
   RotationTimer.setInterval(DEFAULT_ROTATION_DURATION_MS);

   connect(&SettingsProxy, &WallpaperSettingsProxy::SettingsChanged,
           this, &WallpaperService::HandleSettingsChanged);
}

void WallpaperService::RegisterMetaTypes() const
{
   qRegisterMetaType<ViewData>("Wallpaper::ViewData");
}

void WallpaperService::HandleRotationTimeout()
{
   LogInfo("Wallpaper rotation timer is triggering a wallpaper change");
   CalculateCurrentWallpaperData(true);
}

void WallpaperService::HandleSettingsChanged()
{
   const QList<QColor>& colors = SettingsProxy.GetColors();
   if(colors.count() <= CurrentColorsIndex)
   {
      CurrentColorsIndex = 0;
   }

   const QStringList& imagePaths = SettingsProxy.GetPaths();
   if(imagePaths.count() <= CurrentImageIndex)
   {
      CurrentImageIndex = 0;
   }

   if(Schedule::Static == SettingsProxy.GetSchedule())
   {
      RotationTimer.stop();
   }

   CalculateCurrentWallpaperData();
}

void WallpaperService::CalculateCurrentWallpaperData(bool triggeredByRotationTimer)
{
   ViewData data;

   data.Style = SettingsProxy.GetStyle();

   const Schedule schedule = SettingsProxy.GetSchedule();
   switch(schedule)
   {
      case Schedule::Sequence:
      case Schedule::Shuffle:
      {
         CalculateSequenceOrShuffleViewData(data, triggeredByRotationTimer);
         break;
      }
      case Schedule::Static:
      {
         CalculateStaticViewData(data);
         break;
      }
      default:
      {
         LogError(QString("Wallpaper schedule type %1 not handled!")
           .arg(ToString(schedule)));
         break;
      }
   }

   LogInfo(QString("WallpaperDataChanged"));
   emit WallpaperDataChanged(data);
}

void WallpaperService::CalculateNextColor(bool shuffled)
{
   int index = CurrentColorsIndex;
   const QList<QColor>& colors = SettingsProxy.GetColors();

   if(shuffled)
   {
      int tries = 5;
      do
      {
         index = ShuffleRando.Index<QColor>(colors);
         --tries;
      }
      while((tries > 0) &&
               (index == CurrentColorsIndex) &&
               (colors.size() > 1));
   }
   else
   {
      ++index;
      if(index >= colors.count())
      {
         index = 0;
      }
   }

   CurrentColorsIndex = index;
}

void WallpaperService::CalculateNextImage(bool shuffled)
{
   int index = CurrentImageIndex;
   const QStringList& paths = SettingsProxy.GetPaths();

   if(shuffled)
   {
      int tries = 5;
      do
      {
         index = ShuffleRando.Index<QString>(paths);
         --tries;
      }
      while((tries > 0) &&
               (index == CurrentImageIndex) &&
               (paths.size() > 1));
   }
   else
   {
      ++index;
      if(index >= paths.count())
      {
         index = 0;
      }
   }

   CurrentImageIndex = index;
}

void WallpaperService::CalculateSequenceOrShuffleViewData(ViewData& data, bool triggeredByTimer)
{
   const Style style = SettingsProxy.GetStyle();
   const int duration = SettingsProxy.GetDuration();
   const QList<QColor>& colors = SettingsProxy.GetColors();
   const bool shuffle = (Schedule::Shuffle == SettingsProxy.GetSchedule());
   const QStringList& imagePaths = SettingsProxy.GetPaths();
   const QList<Fit>& fits = SettingsProxy.GetFits();

   // TODO: Multi-monitor
   data.AssignedMonitor = 0;

   RotationTimer.start(duration);
   if((Style::DynamicColor == style) ||
       (Style::StaticColor == style))
   {
      data.Fit = Fit::Fill;

      if(triggeredByTimer)
      {
         CalculateNextColor(shuffle);
      }

      if(colors.count() > 0)
      {
         if(CurrentColorsIndex > colors.count())
         {
            LogWarn(QString("Wallpaper CurrentColorsIndex is too high: %1 on range of %2")
                       .arg(CurrentColorsIndex, colors.count()));
            CurrentColorsIndex = 0;
         }

         data.Color = colors[CurrentColorsIndex];
      }
      else
      {
         LogWarn(QString("Style is %1 but no colors are loaded! If this is at bootup, disregard")
                    .arg(ToString(style)));
      }
   }
   else if((Style::Image == style) ||
              (Style::Video == style))
   {
      if(triggeredByTimer)
      {
         CalculateNextImage(shuffle);
      }

      const int numImages = imagePaths.count();
      if(numImages > 0)
      {
         if(CurrentImageIndex > numImages)
         {
            LogWarn(QString("Wallpaper CurrentImageIndex is too high: %1 on range of %2")
                       .arg(CurrentImageIndex, imagePaths.count()));
            CurrentImageIndex = 0;
         }

         data.ImagePath = imagePaths[CurrentImageIndex];
         data.Fit = fits[CurrentImageIndex];
      }
      else
      {
         LogWarn(QString("Style is %1 but no images are loaded! If this is at bootup, disregard.")
                    .arg(ToString(style)));
      }
   }
}

void WallpaperService::CalculateStaticViewData(ViewData& data)
{
   const Style style = SettingsProxy.GetStyle();
   const QList<QColor>& colors = SettingsProxy.GetColors();
   const QStringList& imagePaths = SettingsProxy.GetPaths();
   const QList<Fit>& fits = SettingsProxy.GetFits();

   // TODO: Multi-monitor
   data.AssignedMonitor = 0;
   if((Style::DynamicColor == style) ||
       (Style::StaticColor == style))
   {
      data.Fit = Fit::Fill;

      const int numColors = colors.count();
      if(numColors > 0)
      {
         data.Color = colors[0];
      }
      else
      {
         LogError(QString("Static Schedule selected with %1 style, but no colors are loaded! "
                          "If this is at bootup, disregard.")
                     .arg(ToString(style)));
         data.Color = Qt::black;
      }
   }
}
