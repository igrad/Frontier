#pragma once

#include "WallpaperSettingsProxyInterface.h"

#include <SettingsClient.h>

namespace Wallpaper
{
   class WallpaperSettingsProxy: public WallpaperSettingsProxyInterface
   {
      Q_OBJECT

   public:
      explicit WallpaperSettingsProxy(QObject* parent = nullptr);
      ~WallpaperSettingsProxy() = default;

      QList<QColor> GetColors(const QString& displayID) const override;
      int GetDuration(const QString& displayID) const override;
      QList<Fit> GetFits(const QString& displayID) const override;
      QStringList GetPaths(const QString& displayID) const override;
      Schedule GetSchedule(const QString& displayID) const override;
      Style GetStyle(const QString& displayID) const override;
      DisplayData GetData(const QString& displayID) const override;

   public slots:
      void HandleDisplaySettingWallpaperColorsChanged(const QString& displayID,
                                                      const QVariant& value) override;
      void HandleDisplaySettingWallpaperDurationChanged(const QString& displayID,
                                                        const QVariant& value) override;
      void HandleDisplaySettingWallpaperFitsChanged(const QString& displayID,
                                                    const QVariant& value) override;
      void HandleDisplaySettingWallpaperImagePathsChanged(const QString& displayID,
                                                          const QVariant& value) override;
      void HandleDisplaySettingWallpaperScheduleChanged(const QString& displayID,
                                                        const QVariant& value) override;
      void HandleDisplaySettingWallpaperStyleChanged(const QString& displayID,
                                                     const QVariant& value) override;

      // TODO: public slots to update settings from GUI

   private:
      void SubscribeToDisplaySettings();

      Settings::SettingsClient SettingsClient;
      QMap<QString, DisplayData> Data;
   };
}
