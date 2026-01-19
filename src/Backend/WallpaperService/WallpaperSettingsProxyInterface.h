#pragma once

#include "WallpaperTypes.h"

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

   class WallpaperSettingsProxyInterface: public QObject
   {
      Q_OBJECT

   public:
      ~WallpaperSettingsProxyInterface() = default;

      virtual QList<QColor> GetColors(const QString& displayID) const = 0;
      virtual int GetDuration(const QString& displayID) const = 0;
      virtual QList<Fit> GetFits(const QString& displayID) const = 0;
      virtual QStringList GetPaths(const QString& displayID) const = 0;
      virtual Schedule GetSchedule(const QString& displayID) const = 0;
      virtual Style GetStyle(const QString& displayID) const = 0;
      virtual DisplayData GetData(const QString& displayID) const = 0;

   signals:
      void SettingsChanged(const QString& displayID);

   public slots:
      virtual void HandleDisplaySettingWallpaperColorsChanged(const QString& displayID,
                                                              const QVariant& value) = 0;
      virtual void HandleDisplaySettingWallpaperDurationChanged(const QString& displayID,
                                                                const QVariant& value) = 0;
      virtual void HandleDisplaySettingWallpaperFitsChanged(const QString& displayID,
                                                            const QVariant& value) = 0;
      virtual void HandleDisplaySettingWallpaperImagePathsChanged(const QString& displayID,
                                                                  const QVariant& value) = 0;
      virtual void HandleDisplaySettingWallpaperScheduleChanged(const QString& displayID,
                                                                const QVariant& value) = 0;
      virtual void HandleDisplaySettingWallpaperStyleChanged(const QString& displayID,
                                                             const QVariant& value) = 0;

      // TODO: public slots to update settings from GUI
   };
}
