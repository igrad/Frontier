#pragma once

#include "WallpaperTypes.h"

#include <SettingsClient.h>

#include <QStringList>

namespace Wallpaper
{
   class WallpaperSettingsProxy: public QObject
   {
      Q_OBJECT

   public:
      explicit WallpaperSettingsProxy(QObject* parent = nullptr);
      ~WallpaperSettingsProxy() = default;

      const QList<QColor>& GetColors() const;
      const int GetDuration() const;
      const QList<Fit>& GetFits() const;
      const QStringList& GetPaths() const;
      const Schedule GetSchedule() const;
      const Style GetStyle() const;

   signals:
      void SettingsChanged();

   public slots:
      void HandleSettingWallpaperColorsChanged(const QVariant& value);
      void HandleSettingWallpaperDurationChanged(const QVariant& value);
      void HandleSettingWallpaperFitsChanged(const QVariant& value);
      void HandleSettingWallpaperImagePathsChanged(const QVariant& value);
      void HandleSettingWallpaperScheduleChanged(const QVariant& value);
      void HandleSettingWallpaperStyleChanged(const QVariant& value);

      // TODO: public slots to update settings from GUI

   private:
      void SubscribeToSettings();

      Settings::SettingsClient SettingsClient;

      QList<QColor> CurrentColors;
      int CurrentDuration;
      QList<Fit> CurrentFits;
      QStringList CurrentImagePaths;
      Schedule CurrentSchedule;
      Style CurrentStyle;
   };
}
