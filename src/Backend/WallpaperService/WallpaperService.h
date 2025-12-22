#pragma once

#include "WallpaperTypes.h"

#include <SettingsClient.h>

#include <QObject>
#include <QStringList>

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

   public slots:
      void HandleSettingWallpaperScheduleChanged(const QVariant& value);
      void HandleSettingWallpaperImagePaths(const QVariant& value);
      void HandleSettingWallpaperColor(const QVariant& value);
      void HandleSettingWallpaperDuration(const QVariant& value);
      void HandleSettingWallpaperActiveMode(const QVariant& value);

   private:
      void SubscribeToSettings();

      Settings::SettingsClient Settings;

      WallpaperSchedule Schedule;
      QStringList ImagePaths;
   };
}
