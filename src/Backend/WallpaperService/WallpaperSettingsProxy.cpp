#include "WallpaperSettingsProxy.h"

#include <Log.h>

#include <QColor>
#include <QVariant>

using namespace Settings;
using namespace Wallpaper;

namespace
{
   constexpr int DEFAULT_ROTATION_DURATION_MS = 30000;
   constexpr Fit DEFAULT_WALLPAPER_FIT = Fit::Fill;
   constexpr Style DEFAULT_WALLPAPER_STYLE = Style::StaticColor;
   constexpr Schedule DEFAULT_WALLPAPER_SCHEDULE = Schedule::Static;
   constexpr const char* const SETTINGS_CLIENT_NAME = "WallpaperService";
}

WallpaperSettingsProxy::WallpaperSettingsProxy(QObject* parent)
   : QObject(parent)
   , SettingsClient(SETTINGS_CLIENT_NAME)
   , Data()
{
   SubscribeToDisplaySettings();
}

const QList<QColor>& WallpaperSettingsProxy::GetColors(uint8_t display) const
{
   return Data[display].Colors;
}

const int WallpaperSettingsProxy::GetDuration(uint8_t display) const
{
   return Data[display].Duration;
}

const QList<Fit>& WallpaperSettingsProxy::GetFits(uint8_t display) const
{
   return Data[display].Fits;
}

const QStringList& WallpaperSettingsProxy::GetPaths(uint8_t display) const
{
   return Data[display].ImagePaths;
}

const Schedule WallpaperSettingsProxy::GetSchedule(uint8_t display) const
{
   return Data[display].Schedule;
}

const Style WallpaperSettingsProxy::GetStyle(uint8_t display) const
{
   return Data[display].Style;
}

const DisplayData WallpaperSettingsProxy::GetData(uint8_t display) const
{
   return Data[display];
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperColorsChanged(const QVariant& value,
                                                                        uint8_t display)
{

   Data[display].Colors.clear();

   if(value.canConvert<QStringList>())
   {
      const QStringList strList = value.toStringList();
      for(const QString& str : strList)
      {
         Data[display].Colors.push_back(QColor(str));
      }
   }
   else
   {
      Data[display].Colors.push_back(QColor(value.toString()));
   }

   QStringList strList;
   for(const QColor& color : std::as_const(Data[display].Colors))
   {
      strList.push_back(color.name());
   }

   LogInfo(QString("Wallpaper CurrentColors changed to \"%1\"").arg(strList.join(",")));

   emit SettingsChanged(display);
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperDurationChanged(const QVariant& value,
                                                                          uint8_t display)
{
   if(value.canConvert<int>())
   {
      Data[display].Duration = value.toInt();
      LogInfo(QString("Wallpaper rotation duration changed to %1msec").arg(Data[display].Duration));

      emit SettingsChanged(display);
   }
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperFitsChanged(const QVariant& value,
                                                                      uint8_t display)
{
   const QVariantList list = value.toList();

   bool canConvert = true;
   QList<Fit> fits;
   QStringList strList;
   for(const QVariant& val : std::as_const(list))
   {
      if(!val.canConvert<Fit>())
      {
         canConvert = false;
         break;
      }

      const Fit fit = val.value<Fit>();
      fits.push_back(fit);
      strList.push_back(ToString(fit));
   }

   if(canConvert)
   {
      if(fits != Data[display].Fits)
      {
         Data[display].Fits = fits;

         LogInfo(QString("Wallpaper CurrentFits changed to \"%1\"").arg(strList.join(",")));

         emit SettingsChanged(display);
      }
   }
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperImagePathsChanged(const QVariant& value,
                                                                            uint8_t display)
{
   Data[display].ImagePaths.clear();

   // NOTE: Is this necessary?
   // if(value.canConvert<QStringList>())
   // {
   //    CurrentImagePaths = value.toStringList();
   // }
   // else	// Just pass in a single image
   // {
   Data[display].ImagePaths = value.toStringList();
   // }

   LogInfo(QString("Wallpaper CurrentImagePaths changed to \"%1\"")
              .arg(Data[display].ImagePaths.join(",")));
   emit SettingsChanged(display);
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperScheduleChanged(const QVariant& value,
                                                                          uint8_t display)
{
   const Schedule newSchedule = value.value<Schedule>();

   if(newSchedule != Data[display].Schedule)
   {
      Data[display].Schedule = newSchedule;
      LogInfo(QString("Wallpaper schedule changed to: %1").arg(ToString(Data[display].Schedule)));

      emit SettingsChanged(display);
   }
}

// TODO: Future support for different image styles in the rotation
void WallpaperSettingsProxy::HandleDisplaySettingWallpaperStyleChanged(const QVariant& value,
                                                                       uint8_t display)
{
   if(value.canConvert<Style>())
   {
      const Style style = value.value<Style>();

      if(style != Data[display].Style)
      {
         Data[display].Style = style;

         LogInfo(QString("Wallpaper style changed to %1").arg(ToString(style)));

         emit SettingsChanged(display);
      }
   }
}

void WallpaperSettingsProxy::SubscribeToDisplaySettings()
{
   SettingsClient.SubscribeToDisplaySetting(Setting::WallpaperColors, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::WallpaperDuration, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::WallpaperFits, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::WallpaperImagePaths, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::WallpaperSchedule, this);
   SettingsClient.SubscribeToDisplaySetting(Setting::WallpaperStyle, this);
}
