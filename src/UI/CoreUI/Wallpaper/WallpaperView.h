#pragma once

#include <WallpaperService/WallpaperTypes.h>

#include <DisplayInfo.h>
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
                    const DisplayInfo& info);
      ~WallpaperView();

   public slots:
      void HandleWallpaperDataChanged(const DisplayID& displayID,
                                      const Wallpaper::ViewData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<WallpaperServiceInterface> service);
      void HandleStaticColor(const ViewData& data);
      void HandleDynamicColor(const ViewData& data);
      void HandleImage(const ViewData& data);
      void HandleVideo(const ViewData& data);

      DisplayID Display;
      DisplayInfo Info;
      ViewData CurrentData;
      QStackedLayout* Layout;
      QWidget* Widget;
      QMediaPlayer* MediaPlayer;
      QVideoWidget* VideoWidget;
   };
}
