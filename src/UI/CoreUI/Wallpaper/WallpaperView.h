#pragma once

#include <WallpaperService/WallpaperTypes.h>

#include <Utilities/XThread.h>

#include <QMediaPlayer>
#include <QStackedLayout>
#include <QVideoWidget>
#include <QWidget>

class ShellWindow;

namespace Wallpaper
{
   class WallpaperServiceInterface;

   class WallpaperView: public QWidget
   {
      Q_OBJECT

   public:
      WallpaperView(XThread<WallpaperServiceInterface> service,
                    ShellWindow* window);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(const Wallpaper::ViewData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XThread<WallpaperServiceInterface> service);
      void HandleStaticColor(const ViewData& data);
      void HandleDynamicColor(const ViewData& data);
      void HandleImage(const ViewData& data);
      void HandleVideo(const ViewData& data);

      ViewData CurrentData;
      QStackedLayout* Layout;
      QWidget* Widget;
      QMediaPlayer* MediaPlayer;
      QVideoWidget* VideoWidget;
   };
}
