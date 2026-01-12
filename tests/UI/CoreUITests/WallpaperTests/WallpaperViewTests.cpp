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
      , View(XPtr<WallpaperServiceInterface>(&Service),
             &ShellWindow)
   {

   }

   NiceMock<ShellWindowMock> ShellWindow;
   NiceMock<WallpaperServiceMock> Service;
   WallpaperView View;
};
