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

QList<QColor> WallpaperSettingsProxy::GetColors(const QString& displayID) const
{
   return Data[displayID].Colors;
}

int WallpaperSettingsProxy::GetDuration(const QString& displayID) const
{
   return Data[displayID].Duration;
}

QList<Fit> WallpaperSettingsProxy::GetFits(const QString& displayID) const
{
   return Data[displayID].Fits;
}

QStringList WallpaperSettingsProxy::GetPaths(const QString& displayID) const
{
   return Data[displayID].ImagePaths;
}

Schedule WallpaperSettingsProxy::GetSchedule(const QString& displayID) const
{
   return Data[displayID].Schedule;
}

Style WallpaperSettingsProxy::GetStyle(const QString& displayID) const
{
   return Data[displayID].Style;
}

DisplayData WallpaperSettingsProxy::GetData(const QString& displayID) const
{
   return Data[displayID];
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperColorsChanged(const QVariant& value,
                                                                        const QString& displayID)
{

   Data[displayID].Colors.clear();

   if(value.canConvert<QStringList>())
   {
      const QStringList strList = value.toStringList();
      for(const QString& str : strList)
      {
         Data[displayID].Colors.push_back(QColor(str));
      }
   }
   else
   {
      Data[displayID].Colors.push_back(QColor(value.toString()));
   }

   QStringList strList;
   for(const QColor& color : std::as_const(Data[displayID].Colors))
   {
      strList.push_back(color.name());
   }

   LogInfo(QString("Wallpaper CurrentColors changed to \"%1\"").arg(strList.join(",")));

   emit SettingsChanged(displayID);
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperDurationChanged(const QVariant& value,
                                                                          const QString& displayID)
{
   if(value.canConvert<int>())
   {
      Data[displayID].Duration = value.toInt();
      LogInfo(QString("Wallpaper rotation duration changed to %1msec").arg(Data[displayID].Duration));

      emit SettingsChanged(displayID);
   }
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperFitsChanged(const QVariant& value,
                                                                      const QString& displayID)
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
      if(fits != Data[displayID].Fits)
      {
         Data[displayID].Fits = fits;

         LogInfo(QString("Wallpaper CurrentFits changed to \"%1\"").arg(strList.join(",")));

         emit SettingsChanged(displayID);
      }
   }
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperImagePathsChanged(const QVariant& value,
                                                                            const QString& displayID)
{
   Data[displayID].ImagePaths.clear();

   // NOTE: Is this necessary?
   // if(value.canConvert<QStringList>())
   // {
   //    CurrentImagePaths = value.toStringList();
   // }
   // else	// Just pass in a single image
   // {
   Data[displayID].ImagePaths = value.toStringList();
   // }

   LogInfo(QString("Wallpaper CurrentImagePaths changed to \"%1\"")
              .arg(Data[displayID].ImagePaths.join(",")));
   emit SettingsChanged(displayID);
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperScheduleChanged(const QVariant& value,
                                                                          const QString& displayID)
{
   const Schedule newSchedule = value.value<Schedule>();

   if(newSchedule != Data[displayID].Schedule)
   {
      Data[displayID].Schedule = newSchedule;
      LogInfo(QString("Wallpaper schedule changed to: %1").arg(ToString(Data[displayID].Schedule)));

      emit SettingsChanged(displayID);
   }
}

// TODO: Future support for different image styles in the rotation
void WallpaperSettingsProxy::HandleDisplaySettingWallpaperStyleChanged(const QVariant& value,
                                                                       const QString& displayID)
{
   if(value.canConvert<Style>())
   {
      const Style style = value.value<Style>();

      if(style != Data[displayID].Style)
      {
         Data[displayID].Style = style;

         LogInfo(QString("Wallpaper style changed to %1").arg(ToString(style)));

         emit SettingsChanged(displayID);
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
