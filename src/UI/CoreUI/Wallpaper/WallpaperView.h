#pragma once

#include <WallpaperService/WallpaperTypes.h>

#include <Utilities/XPtr.h>

#include <QMediaPlayer>
#include <QStackedLayout>
#include <QVideoWidget>
#include <QWidget>

class ShellWindowInterface;

namespace Wallpaper
{
   class WallpaperServiceInterface;

   class WallpaperView: public QWidget
   {
      Q_OBJECT

   public:
      WallpaperView(XPtr<WallpaperServiceInterface> service,
                    ShellWindowInterface* window,
                    uint8_t display);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(uint8_t displya, const Wallpaper::ViewData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<WallpaperServiceInterface> service);
      void HandleStaticColor(const ViewData& data);
      void HandleDynamicColor(const ViewData& data);
      void HandleImage(const ViewData& data);
      void HandleVideo(const ViewData& data);

      const uint8_t DisplayNumber;
      ViewData CurrentData;
      QStackedLayout* Layout;
      QWidget* Widget;
      QMediaPlayer* MediaPlayer;
      QVideoWidget* VideoWidget;
   };
}
