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

      QList<QColor> GetColors(const QString& displayID) const;
      int GetDuration(const QString& displayID) const;
      QList<Fit> GetFits(const QString& displayID) const;
      QStringList GetPaths(const QString& displayID) const;
      Schedule GetSchedule(const QString& displayID) const;
      Style GetStyle(const QString& displayID) const;
      DisplayData GetData(const QString& displayID) const;

   signals:
      void SettingsChanged(const QString& displayID);

   public slots:
      void HandleDisplaySettingWallpaperColorsChanged(const QVariant& value,
                                                      const QString& displayID);
      void HandleDisplaySettingWallpaperDurationChanged(const QVariant& value,
                                                        const QString& displayID);
      void HandleDisplaySettingWallpaperFitsChanged(const QVariant& value,
                                                    const QString& displayID);
      void HandleDisplaySettingWallpaperImagePathsChanged(const QVariant& value,
                                                          const QString& displayID);
      void HandleDisplaySettingWallpaperScheduleChanged(const QVariant& value,
                                                        const QString& displayID);
      void HandleDisplaySettingWallpaperStyleChanged(const QVariant& value,
                                                     const QString& displayID);

      // TODO: public slots to update settings from GUI

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QMap<QString, DisplayData> Data;
   };
}
