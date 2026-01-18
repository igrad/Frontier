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

QList<QColor> WallpaperSettingsProxy::GetColors(QString id) const
{
   return Data[id].Colors;
}

int WallpaperSettingsProxy::GetDuration(QString id) const
{
   return Data[id].Duration;
}

QList<Fit> WallpaperSettingsProxy::GetFits(QString id) const
{
   return Data[id].Fits;
}

QStringList WallpaperSettingsProxy::GetPaths(QString id) const
{
   return Data[id].ImagePaths;
}

Schedule WallpaperSettingsProxy::GetSchedule(QString id) const
{
   return Data[id].Schedule;
}

Style WallpaperSettingsProxy::GetStyle(QString id) const
{
   return Data[id].Style;
}

DisplayData WallpaperSettingsProxy::GetData(QString id) const
{
   return Data[id];
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperColorsChanged(const QVariant& value,
                                                                        QString id)
{

   Data[id].Colors.clear();

   if(value.canConvert<QStringList>())
   {
      const QStringList strList = value.toStringList();
      for(const QString& str : strList)
      {
         Data[id].Colors.push_back(QColor(str));
      }
   }
   else
   {
      Data[id].Colors.push_back(QColor(value.toString()));
   }

   QStringList strList;
   for(const QColor& color : std::as_const(Data[id].Colors))
   {
      strList.push_back(color.name());
   }

   LogInfo(QString("Wallpaper CurrentColors changed to \"%1\"").arg(strList.join(",")));

   emit SettingsChanged(id);
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperDurationChanged(const QVariant& value,
                                                                          QString id)
{
   if(value.canConvert<int>())
   {
      Data[id].Duration = value.toInt();
      LogInfo(QString("Wallpaper rotation duration changed to %1msec").arg(Data[id].Duration));

      emit SettingsChanged(id);
   }
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperFitsChanged(const QVariant& value,
                                                                      QString id)
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
      if(fits != Data[id].Fits)
      {
         Data[id].Fits = fits;

         LogInfo(QString("Wallpaper CurrentFits changed to \"%1\"").arg(strList.join(",")));

         emit SettingsChanged(id);
      }
   }
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperImagePathsChanged(const QVariant& value,
                                                                            QString id)
{
   Data[id].ImagePaths.clear();

   // NOTE: Is this necessary?
   // if(value.canConvert<QStringList>())
   // {
   //    CurrentImagePaths = value.toStringList();
   // }
   // else	// Just pass in a single image
   // {
   Data[id].ImagePaths = value.toStringList();
   // }

   LogInfo(QString("Wallpaper CurrentImagePaths changed to \"%1\"")
              .arg(Data[id].ImagePaths.join(",")));
   emit SettingsChanged(id);
}

void WallpaperSettingsProxy::HandleDisplaySettingWallpaperScheduleChanged(const QVariant& value,
                                                                          QString id)
{
   const Schedule newSchedule = value.value<Schedule>();

   if(newSchedule != Data[id].Schedule)
   {
      Data[id].Schedule = newSchedule;
      LogInfo(QString("Wallpaper schedule changed to: %1").arg(ToString(Data[id].Schedule)));

      emit SettingsChanged(id);
   }
}

// TODO: Future support for different image styles in the rotation
void WallpaperSettingsProxy::HandleDisplaySettingWallpaperStyleChanged(const QVariant& value,
                                                                       QString id)
{
   if(value.canConvert<Style>())
   {
      const Style style = value.value<Style>();

      if(style != Data[id].Style)
      {
         Data[id].Style = style;

         LogInfo(QString("Wallpaper style changed to %1").arg(ToString(style)));

         emit SettingsChanged(id);
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
