#include "ShellWindow.h"
#include <ArgParser.h>

#include <Log.h>

namespace
{
   constexpr int DEV_WINDOW_WIDTH = 1920;
   constexpr int DEV_WINDOW_HEIGHT = 1080;
}

ShellWindow::ShellWindow(QWidget* parent)
   : QWidget(parent)
{
   {
      typedef ArgParser::Arg Arg;
      if(ArgParser::GetArgAsBool(Arg::Dev, false))
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
}
