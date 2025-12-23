#pragma once

#include <WallpaperService/WallpaperTypes.h>

namespace Wallpaper
{
   class WallpaperService;

   // TODO: Change QObject here to a widget or frame or something
   class WallpaperView: public QObject
   {
      Q_OBJECT

   public:
      explicit WallpaperView(WallpaperService* service);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(const WallpaperData& data);

   private:
      void ConnectToServiceSignals(WallpaperService* service);

      WallpaperData CurrentData;
   };
}
