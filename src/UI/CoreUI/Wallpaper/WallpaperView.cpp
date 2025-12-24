#include "WallpaperView.h"

#include <ShellWindow.h>
#include <WallpaperService.h>

using namespace Wallpaper;

WallpaperView::WallpaperView(std::unique_ptr<WallpaperService>& service,
                             std::unique_ptr<ShellWindow>& window)
   : QWidget(window.get())
   , Window(window)
   , CurrentData()
   , Layout(nullptr)
   , Widget(nullptr)
   , MediaPlayer(nullptr)
   , VideoWidget(nullptr)
{
   CreateUI();
   ConnectToServiceSignals(service);
}

WallpaperView::~WallpaperView()
{

}

void WallpaperView::HandleWallpaperDataChanged(const WallpaperData& data)
{
   CurrentData = data;

   if(MediaPlayer->isPlaying() &&
       (ImageType::Video != data.ImageType))
   {
      MediaPlayer->stop();
      MediaPlayer->setSource(QUrl());
   }

   switch(data.ImageType)
   {
   case ImageType::StaticColor:
      HandleStaticColor(data);
      break;
   case ImageType::DynamicColor:
      HandleDynamicColor(data);
      break;
   case ImageType::Image:
      HandleImage(data);
      break;
   case ImageType::Video:
      HandleVideo(data);
      break;
   default:
      LogWarn(QString("Unhandled ImageType: %1. Ignoring.").arg(ToString(data.ImageType)));
   }
}

void WallpaperView::CreateUI()
{
   Layout.reset(new QStackedLayout(this));

   Widget.reset(new QWidget(this));
   Layout->addWidget(Widget.get());

   MediaPlayer.reset(new QMediaPlayer(this));
   VideoWidget.reset(new QVideoWidget(this));
   MediaPlayer->setVideoOutput(VideoWidget.get());
   Layout->addWidget(VideoWidget.get());

   setLayout(Layout.get());
   Layout->setGeometry({0, 0, 1920, 1080});
   setGeometry({0, 0, 1920, 1080});

   WallpaperData data;
   data.AssignedMonitor = 0;
   data.Colors = {Qt::blue};
   data.ImagePath = "";
   data.ImageType = ImageType::StaticColor;
   data.Mode = DisplayMode::Fill;
   HandleWallpaperDataChanged(data);
   show();
}

void WallpaperView::ConnectToServiceSignals(std::unique_ptr<WallpaperService>& service)
{
   if(nullptr != service)
   {
      // Prevents a clang warning about bitwise OR (|) op on these connection types
      // NOLINTNEXTLINE
      const auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection |
                                                        Qt::QueuedConnection);

      connect(service.get(), &WallpaperService::WallpaperDataChanged,
              this, &WallpaperView::HandleWallpaperDataChanged,
              conn);
   }
}

void WallpaperView::HandleStaticColor(const WallpaperData& data)
{
   Layout->setCurrentWidget(Widget.get());
   const QString color = (0 < data.Colors.count()) ? data.Colors[0].name() : "#000000";
   Widget->setStyleSheet(QString("background-color: %1")
                               .arg(color));
   Widget->show();
}

void WallpaperView::HandleDynamicColor(const WallpaperData& data)
{
   Layout->setCurrentWidget(Widget.get());
}

void WallpaperView::HandleImage(const WallpaperData& data)
{
   Layout->setCurrentWidget(Widget.get());
}

void WallpaperView::HandleVideo(const WallpaperData& data)
{

}
