#pragma once

#include <WallpaperService/WallpaperTypes.h>

#include <QMediaPlayer>
#include <QStackedLayout>
#include <QVideoWidget>
#include <QWidget>

namespace Wallpaper
{
   class WallpaperService;

   // TODO: Change QObject here to a widget or frame or something
   class WallpaperView: public QWidget
   {
      Q_OBJECT

   public:
      explicit WallpaperView(std::unique_ptr<WallpaperService>& service);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(const Wallpaper::WallpaperData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(std::unique_ptr<WallpaperService>& service);
      void HandleStaticColor(const WallpaperData& data);
      void HandleDynamicColor(const WallpaperData& data);
      void HandleImage(const WallpaperData& data);
      void HandleVideo(const WallpaperData& data);

      WallpaperData CurrentData;
      std::unique_ptr<QStackedLayout> Layout;
      std::unique_ptr<QWidget> Widget;
      std::unique_ptr<QMediaPlayer> MediaPlayer;
      std::unique_ptr<QVideoWidget> VideoWidget;
   };
}
