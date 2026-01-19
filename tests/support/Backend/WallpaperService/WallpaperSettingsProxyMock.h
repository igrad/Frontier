#pragma once

#include <WallpaperSettingsProxyInterface.h>

#include <gmock/gmock.h>

namespace Wallpaper
{
   class WallpaperSettingsProxyMock: public WallpaperSettingsProxyInterface
   {
      Q_OBJECT

   public:
      WallpaperSettingsProxyMock() = default;
      ~WallpaperSettingsProxyMock() = default;

      void EmitSettingsChanged(const QString& displayID);

      MOCK_METHOD(QList<QColor>, GetColors, (const QString&), (const override));
      MOCK_METHOD(int, GetDuration, (const QString& displayID), (const override));
      MOCK_METHOD(QList<Fit>, GetFits, (const QString& displayID), (const override));
      MOCK_METHOD(QStringList, GetPaths, (const QString& displayID), (const override));
      MOCK_METHOD(Schedule, GetSchedule, (const QString& displayID), (const override));
      MOCK_METHOD(Style, GetStyle, (const QString& displayID), (const override));
      MOCK_METHOD(DisplayData, GetData, (const QString& displayID), (const override));

      MOCK_METHOD(void,
                  HandleDisplaySettingWallpaperColorsChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingWallpaperDurationChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingWallpaperFitsChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingWallpaperImagePathsChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingWallpaperScheduleChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingWallpaperStyleChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
   };
}
