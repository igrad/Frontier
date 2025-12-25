#pragma once

#include <WallpaperService/WallpaperTypes.h>

#include <QMediaPlayer>
#include <QStackedLayout>
#include <QVideoWidget>
#include <QWidget>

class ShellWindow;

namespace Wallpaper
{
   class WallpaperService;

   class WallpaperView: public QWidget
   {
      Q_OBJECT

   public:
      WallpaperView(WallpaperService* service,
                    ShellWindow* window);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(const Wallpaper::WallpaperData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(WallpaperService* service);
      void HandleStaticColor(const WallpaperData& data);
      void HandleDynamicColor(const WallpaperData& data);
      void HandleImage(const WallpaperData& data);
      void HandleVideo(const WallpaperData& data);

      WallpaperData CurrentData;
      QStackedLayout* Layout;
      QWidget* Widget;
      QMediaPlayer* MediaPlayer;
      QVideoWidget* VideoWidget;
   };
}
