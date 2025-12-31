#include "WallpaperView.h"

#include <ShellWindow.h>
#include <WallpaperService.h>

using namespace Wallpaper;

WallpaperView::WallpaperView(WallpaperService* service,
                             ShellWindow* window)
   : QWidget(window)
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
   if(MediaPlayer)
   {
      MediaPlayer->stop();
   }

   Layout->removeWidget(VideoWidget);

   // if(VideoWidget)
   // {
   // // VideoWidget->deleteLater();
   // }

   // if(MediaPlayer)
   // {
   // MediaPlayer->deleteLater();
   // }

   // Widget->deleteLater();

   // Layout->deleteLater();
}

void WallpaperView::HandleWallpaperDataChanged(const ViewData& data)
{
   CurrentData = data;

   if(MediaPlayer->isPlaying() &&
       (Style::Video != data.Style))
   {
      MediaPlayer->stop();
      MediaPlayer->setSource(QUrl());
   }

   switch(data.Style)
   {
   case Style::StaticColor:
      HandleStaticColor(data);
      break;
   case Style::DynamicColor:
      HandleDynamicColor(data);
      break;
   case Style::Image:
      HandleImage(data);
      break;
   case Style::Video:
      HandleVideo(data);
      break;
   default:
      LogWarn(QString("Unhandled Style: %1. Ignoring.").arg(ToString(data.Style)));
   }
}

void WallpaperView::CreateUI()
{
   Layout = new QStackedLayout(this);

   Widget = new QWidget(this);
   Layout->addWidget(Widget);

   MediaPlayer = new QMediaPlayer(this);
   VideoWidget = new QVideoWidget(this);
   MediaPlayer->setVideoOutput(VideoWidget);
   Layout->addWidget(VideoWidget);

   setLayout(Layout);
   Layout->setGeometry({0, 0, 1920, 1080});
   setGeometry({0, 0, 1920, 1080});

   ViewData data;
   data.AssignedMonitor = 0;
   data.Color = Qt::blue;
   data.ImagePath = "";
   data.Style = Style::StaticColor;
   data.Fit = Fit::Fill;
   HandleWallpaperDataChanged(data);
   show();
}

void WallpaperView::ConnectToServiceSignals(WallpaperService* service)
{
   if(nullptr != service)
   {
      // Prevents a clang warning about bitwise OR (|) op on these connection types
      // NOLINTNEXTLINE
      const auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection |
                                                        Qt::QueuedConnection);

      connect(service, &WallpaperService::WallpaperDataChanged,
              this, &WallpaperView::HandleWallpaperDataChanged,
              conn);
   }
}

void WallpaperView::HandleStaticColor(const ViewData& data)
{
   Layout->setCurrentWidget(Widget);
   const QString color = (data.Color != QColor()) ? data.Color.name() : "#000000";
   Widget->setStyleSheet(QString("background-color: %1")
                               .arg(color));
   Widget->show();
}

void WallpaperView::HandleDynamicColor(const ViewData& data)
{
   Layout->setCurrentWidget(Widget);
}

void WallpaperView::HandleImage(const ViewData& data)
{
   Layout->setCurrentWidget(Widget);
}

void WallpaperView::HandleVideo(const ViewData& data)
{

}
