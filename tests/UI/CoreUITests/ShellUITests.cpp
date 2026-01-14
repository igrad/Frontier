#include <ShellUI.h>

#include <TaskBarService/TaskBarServiceMock.h>
#include <WallpaperService/WallpaperServiceMock.h>

#include <TestMacros.h>

#include <gtest/gtest.h>

using namespace testing;

class ShellUITest: public Test
{
public:
   ShellUITest()
      : TaskBarService()
      , WallpaperService()
      , TheDisplayInfo()
      , UI(nullptr)
   {

   }

   void SetUpWithDisplayInfo(const DisplayInfo& info)
   {
      TheDisplayInfo = info;
      SetUpUI();
   }

   void SetUpUI()
   {
      UI.reset(new ShellUI(XPtr<TaskBar::TaskBarServiceInterface>(&TaskBarService),
                           XPtr<Wallpaper::WallpaperServiceInterface>(&WallpaperService),
                           TheDisplayInfo));
   }

   NiceMock<TaskBar::TaskBarServiceMock> TaskBarService;
   NiceMock<Wallpaper::WallpaperServiceMock> WallpaperService;
   DisplayInfo TheDisplayInfo;
   std::unique_ptr<ShellUI> UI;
};
