#pragma once

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

   private slots:
      // Need to decide what the settings for wallpapers will look like
      // A schedule? shuffle, static, sequenced
      void HandleSettingWallpaperScheduleChanged(const QVariant& value);

   private:
      void SubscribeToSettings();

      Settings::SettingsClient Settings;
   };
}
