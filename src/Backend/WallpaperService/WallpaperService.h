#pragma once

#include "WallpaperTypes.h"

#include <SettingsClient.h>

#include <QObject>

namespace Wallpaper
{
   class WallpaperService: public QObject
   {
      Q_OBJECT

   public:
      WallpaperService();
      ~WallpaperService();

   signals:
      void WallpaperScheduleChanged(WallpaperSchedule schedule);

   private slots:
      // Need to decide what the settings for wallpapers will look like
      // A schedule? shuffle, static, sequenced
      void HandleSettingWallpaperScheduleChanged(const QVariant& value);

   private:
      void SubscribeToSettings();

      Settings::SettingsClient Settings;

      WallpaperSchedule Schedule;
   };
}
