#pragma once

#include "WallpaperTypes.h"

#include <SettingsService/SettingsClient.h>
#include <Utilities/Rando.h>

#include <QColor>
#include <QObject>
#include <QStringList>
#include <QTimer>

namespace Wallpaper
{
   class WallpaperService: public QObject
   {
      Q_OBJECT

   public:
      explicit WallpaperService(QObject* parent = nullptr);
      ~WallpaperService() = default;

      void RegisterMetaTypes() const;

   signals:
      void WallpaperDataChanged(const Wallpaper::ViewData& data);

   public slots:
      void HandleSettingWallpaperColorsChanged(const QVariant& value);
      void HandleSettingWallpaperDurationChanged(const QVariant& value);
      void HandleSettingWallpaperFitChanged(const QVariant& value);
      void HandleSettingWallpaperImagePathsChanged(const QVariant& value);
      void HandleSettingWallpaperScheduleChanged(const QVariant& value);
      void HandleSettingWallpaperStyleChanged(const QVariant& value);

   private slots:
      void HandleRotationTimeout();

   private:
      void SubscribeToSettings();
      void CalculateCurrentWallpaperData(bool triggeredByRotationTimer = false);
      void CalculateNextColor(bool shuffled);

      Settings::SettingsClient Settings;

      QList<QColor> CurrentColors;
      int CurrentDuration;
      Fit CurrentFit;
      QStringList CurrentImagePaths;
      Schedule CurrentSchedule;
      Style CurrentStyle;

      int CurrentColorsIndex;
      QTimer RotationTimer;
      Rando ShuffleRando;
   };
}
