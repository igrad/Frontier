#include "WallpaperServiceWorker.h"

#include <WallpaperSettingsProxyInterface.h>

using namespace Wallpaper;

namespace
{
   constexpr int DEFAULT_ROTATION_DURATION_MS = 30000;
}

WallpaperServiceWorker::WallpaperServiceWorker(const DisplayInfo& info,
                                               WallpaperSettingsProxyInterface* settingsProxy,
                                               QObject* parent)
   : QObject(parent)
   , Display(info.ID)
   , Info(info)
   , SettingsProxy(settingsProxy)
   , Data()
   , CurrentColorsIndex(0)
   , CurrentImageIndex(0)
   , RotationTimer(this)
   , ShuffleRando()
{
   RotationTimer.SetInterval(DEFAULT_ROTATION_DURATION_MS);

   connect(&RotationTimer, &Timer::timeout,
           this, &WallpaperServiceWorker::HandleRotationTimeout);
}

WallpaperServiceWorker::WallpaperServiceWorker(const WallpaperServiceWorker& other)
   : QObject(other.parent())
   , Display(other.GetDisplayID())
   , Info(other.GetDisplayInfo())
   , SettingsProxy(other.SettingsProxy)
   , Data(other.Data)
   , CurrentColorsIndex(other.CurrentColorsIndex)
   , CurrentImageIndex(other.CurrentImageIndex)
   , RotationTimer(other.RotationTimer)
   , ShuffleRando(other.ShuffleRando)
{

}

QString WallpaperServiceWorker::GetDisplayID() const
{
   return Display;
}

DisplayInfo WallpaperServiceWorker::GetDisplayInfo() const
{
   return Info;
}

bool WallpaperServiceWorker::operator==(const WallpaperServiceWorker& rhs) const
{
   return Display == rhs.GetDisplayID();
}

WallpaperServiceWorker& WallpaperServiceWorker::operator=(const WallpaperServiceWorker& rhs)
{
   setParent(rhs.parent());
   Display = rhs.Display;
   Info = rhs.Info;
   SettingsProxy = rhs.SettingsProxy;
   Data = rhs.Data;
   CurrentColorsIndex = rhs.CurrentColorsIndex;
   CurrentImageIndex = rhs.CurrentImageIndex;
   RotationTimer = rhs.RotationTimer;
   ShuffleRando = rhs.ShuffleRando;
   return *this;
}

void WallpaperServiceWorker::HandleRotationTimeout()
{
   LogInfo("Wallpaper rotation timer is triggering a wallpaper change");
   CalculateCurrentWallpaperData(true);
}

void WallpaperServiceWorker::HandleSettingsChanged()
{
   const QList<QColor>& colors = SettingsProxy->GetColors(Display);
   if(colors.count() <= CurrentColorsIndex)
   {
      CurrentColorsIndex = 0;
   }

   const QStringList& imagePaths = SettingsProxy->GetPaths(Display);
   if(imagePaths.count() <= CurrentImageIndex)
   {
      CurrentImageIndex = 0;
   }

   if(Schedule::Static == SettingsProxy->GetSchedule(Display))
   {
      RotationTimer.Stop();
   }
   else
   {
      RotationTimer.Start(SettingsProxy->GetDuration(Display));
   }

   CalculateCurrentWallpaperData();
}

void WallpaperServiceWorker::HandleDisplayConfigChanged(const DisplayInfo& info)
{
   // TODO
}

void WallpaperServiceWorker::HandleDisplayRemoved(const DisplayInfo& info)
{
   // TODO
   // Probably have to notify the view somehow?
}

void WallpaperServiceWorker::CalculateCurrentWallpaperData(bool triggeredByRotationTimer)
{
   ViewData data;
   Data = data;

   Data.Style = SettingsProxy->GetStyle(Display);

   // Temporary workaround, I don't like it for prod
   if(Style::None == Data.Style)
   {
      Data.Style = Style::StaticColor;
   }

   const Schedule schedule = SettingsProxy->GetSchedule(Display);
   switch(schedule)
   {
      case Schedule::Sequence:
      case Schedule::Shuffle:
      {
         CalculateSequenceOrShuffleViewData(triggeredByRotationTimer);
         break;
      }
      case Schedule::Static:
      {
         CalculateStaticViewData();
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
   emit WallpaperViewDataChanged(Info.ID, Data);
}

void WallpaperServiceWorker::CalculateNextColor(bool shuffled)
{
   int index = CurrentColorsIndex;
   const QList<QColor>& colors = SettingsProxy->GetColors(Display);

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

void WallpaperServiceWorker::CalculateNextImage(bool shuffled)
{
   int index = CurrentImageIndex;
   const QStringList& paths = SettingsProxy->GetPaths(Display);

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

void WallpaperServiceWorker::CalculateSequenceOrShuffleViewData(bool triggeredByTimer)
{
   const Style style = SettingsProxy->GetStyle(Display);
   const int duration = SettingsProxy->GetDuration(Display);

   // TODO: Multi-monitor
   Data.AssignedMonitor = 0;

   RotationTimer.Start(duration);
   if((Style::DynamicColor == style) ||
       (Style::StaticColor == style))
   {
      ProcessColorStyle(triggeredByTimer);
   }
   else if((Style::Image == style) ||
              (Style::Video == style))
   {
      ProcessImageStyle(triggeredByTimer);
   }
}

void WallpaperServiceWorker::CalculateStaticViewData()
{
   const Style style = SettingsProxy->GetStyle(Display);
   const QList<QColor>& colors = SettingsProxy->GetColors(Display);
   const QStringList& imagePaths = SettingsProxy->GetPaths(Display);
   const QList<Fit>& fits = SettingsProxy->GetFits(Display);

   // TODO: Multi-monitor
   Data.AssignedMonitor = 0;
   if((Style::DynamicColor == style) ||
       (Style::StaticColor == style))
   {
      Data.Fit = Fit::Fill;

      const int numColors = colors.count();
      if(numColors > 0)
      {
         Data.Color = colors[0];
      }
      else
      {
         LogError(QString("Static Schedule selected with %1 style, but no colors are loaded! "
                          "If this is at bootup, disregard.")
                     .arg(ToString(style)));
         Data.Color = Qt::black;
      }
   }
}

void WallpaperServiceWorker::ProcessColorStyle(bool triggeredByTimer)
{
   const Style style = SettingsProxy->GetStyle(Display);
   // const int duration = SettingsProxy->GetDuration();
   const QList<QColor>& colors = SettingsProxy->GetColors(Display);
   const bool shuffle = (Schedule::Shuffle == SettingsProxy->GetSchedule(Display));
   // const QList<Fit>& fits = SettingsProxy->GetFits();

   Data.Fit = Fit::Fill;

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

      Data.Color = colors[CurrentColorsIndex];
   }
   else
   {
      LogWarn(QString("Style is %1 but no colors are loaded! If this is at bootup, disregard")
                 .arg(ToString(style)));
   }
}

void WallpaperServiceWorker::ProcessImageStyle(bool triggeredByTimer)
{
   const Style style = SettingsProxy->GetStyle(Display);
   // const int duration = SettingsProxy->GetDuration();
   const QList<QColor>& colors = SettingsProxy->GetColors(Display);
   const bool shuffle = (Schedule::Shuffle == SettingsProxy->GetSchedule(Display));
   const QStringList& imagePaths = SettingsProxy->GetPaths(Display);
   const QList<Fit>& fits = SettingsProxy->GetFits(Display);

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

      Data.ImagePath = imagePaths[CurrentImageIndex];
      Data.Fit = fits[CurrentImageIndex];
   }
   else
   {
      LogWarn(QString("Style is %1 but no images are loaded! If this is at bootup, disregard.")
                 .arg(ToString(style)));
   }
}
