#include <Wallpaper/WallpaperView.h>

#include <WallpaperService/WallpaperServiceMock.h>
#include <ShellWindowMock.h>

#include <TestMacros.h>

#include <gtest/gtest.h>

using namespace testing;
using namespace Wallpaper;

class WallpaperViewTest: public Test
{
   WallpaperViewTest()
      : Service()
      , DisplayNum(0)
      , View(XPtr<WallpaperServiceInterface>(&Service),
             &ShellWindow,
             DisplayNum)
   {

   }

   NiceMock<ShellWindowMock> ShellWindow;
   NiceMock<WallpaperServiceMock> Service;
   uint8_t DisplayNum;
   WallpaperView View;
};
