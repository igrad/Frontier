#include "ShellWindow.h"
#include <ArgParser.h>

namespace
{
   constexpr int DEV_WINDOW_WIDTH = 1920;
   constexpr int DEV_WINDOW_HEIGHT = 1080;
}

ShellWindow::ShellWindow(QWidget *parent)
   : QMainWindow(parent)
{
   QWidget window;
   {
      typedef ArgParser::Arg Arg;
      if(ArgParser::GetArgAsBool(Arg::Dev, false))
      {
         window.setWindowTitle("Frontier - Dev Mode");
         const int devWindowWidth = ArgParser::GetArgAsInt(Arg::DevWindowWidth,
                                                           DEV_WINDOW_WIDTH);
         const int devWindowHeight = ArgParser::GetArgAsInt(Arg::DevWindowHeight,
                                                            DEV_WINDOW_HEIGHT);

         window.resize(devWindowWidth, devWindowHeight);
         window.show();
      }
      else
      {
         window.setWindowTitle("Frontier");
         window.showFullScreen();
      }
   }

   setCentralWidget(&window);
}
