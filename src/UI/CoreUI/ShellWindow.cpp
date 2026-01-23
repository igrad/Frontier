#include "ShellWindow.h"

#include <ArgParser.h>
#include <Log.h>

typedef ArgParser::Arg Arg;
namespace
{
   constexpr int DEV_WINDOW_WIDTH = 1920;
   constexpr int DEV_WINDOW_HEIGHT = 1080;
}

ShellWindow::ShellWindow(const DisplayInfo& info, QWidget* parent)
   : Info(info)
{
   setParent(parent);

   if(ArgParser::RunningWithEnterprise())
   {
      // NOLINTNEXTLINE
      HandleDisplayConfigChanged(info);

      show();
   }
   else if(ArgParser::GetArgAsBool(Arg::Dev, false))
   {
      LogInfo("Launching in dev mode");

      setWindowTitle("Frontier - Dev Mode");
      const int devWindowWidth = ArgParser::GetArgAsInt(Arg::DevWindowWidth,
                                                        DEV_WINDOW_WIDTH);
      const int devWindowHeight = ArgParser::GetArgAsInt(Arg::DevWindowHeight,
                                                         DEV_WINDOW_HEIGHT);

      resize(devWindowWidth, devWindowHeight);
      show();
   }
   else
   {
      setWindowTitle("Frontier");
      setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
      showFullScreen();
   }
}


void ShellWindow::HandleDisplayConfigChanged(const DisplayInfo& info)
{
   const QString title = QString("Frontier - Display #%1 (%2 x %3)")
                            .arg(QString::number(info.Number),
                                 QString::number(info.Rect.width()),
                                 QString::number(info.Rect.height()));
   setWindowTitle(title);
   resize(info.Rect.width(), info.Rect.height());
}

void ShellWindow::closeEvent(QCloseEvent* event)
{
   Q_UNUSED(event);

   emit Closed();
}
