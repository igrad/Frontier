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
      explicit WallpaperView(std::unique_ptr<WallpaperService>& service);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(const WallpaperData& data);

   private:
      void ConnectToServiceSignals(std::unique_ptr<WallpaperService>& service);

      WallpaperData CurrentData;
   };
}
