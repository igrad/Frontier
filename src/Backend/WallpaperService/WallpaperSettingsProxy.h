#pragma once

#include "WallpaperTypes.h"

#include <SettingsClient.h>

#include <QStringList>

namespace Wallpaper
{
   struct DisplayData
   {
      QList<QColor> Colors;
      int Duration;
      QList<Fit> Fits;
      QStringList ImagePaths;
      Schedule Schedule;
      Style Style;
   };

   class WallpaperSettingsProxy: public QObject
   {
      Q_OBJECT

   public:
      explicit WallpaperSettingsProxy(QObject* parent = nullptr);
      ~WallpaperSettingsProxy() = default;

      const QList<QColor>& GetColors(uint8_t display) const;
      const int GetDuration(uint8_t display) const;
      const QList<Fit>& GetFits(uint8_t display) const;
      const QStringList& GetPaths(uint8_t display) const;
      const Schedule GetSchedule(uint8_t display) const;
      const Style GetStyle(uint8_t display) const;
      const DisplayData GetData(uint8_t display) const;

   signals:
      void SettingsChanged(uint8_t display);

   public slots:
      void HandleDisplaySettingWallpaperColorsChanged(const QVariant& value,
                                                      uint8_t display);
      void HandleDisplaySettingWallpaperDurationChanged(const QVariant& value,
                                                        uint8_t display);
      void HandleDisplaySettingWallpaperFitsChanged(const QVariant& value,
                                                    uint8_t display);
      void HandleDisplaySettingWallpaperImagePathsChanged(const QVariant& value,
                                                          uint8_t display);
      void HandleDisplaySettingWallpaperScheduleChanged(const QVariant& value,
                                                        uint8_t display);
      void HandleDisplaySettingWallpaperStyleChanged(const QVariant& value,
                                                     uint8_t display);

      // TODO: public slots to update settings from GUI

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QList<DisplayData> Data;
   };
}
