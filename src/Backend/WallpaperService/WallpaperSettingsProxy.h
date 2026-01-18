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

      QList<QColor> GetColors(QString id) const;
      int GetDuration(QString id) const;
      QList<Fit> GetFits(QString id) const;
      QStringList GetPaths(QString id) const;
      Schedule GetSchedule(QString id) const;
      Style GetStyle(QString id) const;
      DisplayData GetData(QString id) const;

   signals:
      void SettingsChanged(QString id);

   public slots:
      void HandleDisplaySettingWallpaperColorsChanged(const QVariant& value,
                                                      QString id);
      void HandleDisplaySettingWallpaperDurationChanged(const QVariant& value,
                                                        QString id);
      void HandleDisplaySettingWallpaperFitsChanged(const QVariant& value,
                                                    QString id);
      void HandleDisplaySettingWallpaperImagePathsChanged(const QVariant& value,
                                                          QString id);
      void HandleDisplaySettingWallpaperScheduleChanged(const QVariant& value,
                                                        QString id);
      void HandleDisplaySettingWallpaperStyleChanged(const QVariant& value,
                                                     QString id);

      // TODO: public slots to update settings from GUI

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QMap<QString, DisplayData> Data;
   };
}
