#include <TaskBarServiceWorker.h>

#include <TaskBarSettingsProxyMock.h>

#include <QSignalSpy>

#include <TestMacros.h>

using namespace TaskBar;
using namespace testing;

namespace
{
   constexpr const char* const DISPLAY_ID = "DISPLAY_ID";
   constexpr const int RECT_X = 0;
   constexpr const int RECT_Y = 0;
   constexpr const int RECT_W = 1920;
   constexpr const int RECT_H = 1080;
}

class TaskBarServiceWorkerTest: public Test
{
public:
   TaskBarServiceWorkerTest()
      : Info()
      , SettingsProxyMock()
      , Worker(nullptr)
   {

   }

   void SetUpDefaultInfo()
   {
      Info.ID = DISPLAY_ID;
      Info.Number = 1;
      Info.DisplayName = DISPLAY_ID;
      Info.SessionName = DISPLAY_ID;
      Info.Rect = {RECT_X, RECT_Y, RECT_W, RECT_H};
      Info.IsPrimary = true;
   }

   void SetUpWorker()
   {
      Worker.reset(new TaskBarServiceWorker(Info, &SettingsProxyMock));
   }

   void SetUpDefaultWorker()
   {
      SetUpDefaultInfo();
      Worker.reset(new TaskBarServiceWorker(Info, &SettingsProxyMock));
   }

   DisplayInfo Info;
   NiceMock<TaskBarSettingsProxyMock> SettingsProxyMock;
   std::unique_ptr<TaskBarServiceWorker> Worker;
};
