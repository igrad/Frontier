#include <WallpaperServiceWorker.h>

#include <WallpaperSettingsProxyMock.h>

#include <QSignalSpy>

#include <TestMacros.h>

using namespace testing;
using namespace Wallpaper;

namespace
{
   constexpr const char* const DISPLAY_ID = "DISPLAY_ID";
   constexpr const int RECT_X = 0;
   constexpr const int RECT_Y = 0;
   constexpr const int RECT_W = 1920;
   constexpr const int RECT_H = 1080;
}

class WallpaperServiceWorkerTest: public Test
{
public:
   WallpaperServiceWorkerTest()
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
      Worker.reset(new WallpaperServiceWorker(Info, &SettingsProxyMock));
   }

   void SetUpDefaultWorker()
   {
      SetUpDefaultInfo();
      Worker.reset(new WallpaperServiceWorker(Info, &SettingsProxyMock));
   }

   DisplayInfo Info;
   NiceMock<WallpaperSettingsProxyMock> SettingsProxyMock;
   std::unique_ptr<WallpaperServiceWorker> Worker;
};

TEST_F(WallpaperServiceWorkerTest, HandleRotationTimeout1)
{
   SetUpDefaultWorker();

   QSignalSpy spy(Worker.get(), &WallpaperServiceWorker::WallpaperViewDataChanged);

   const Style style = Style::DynamicColor;
   const int duration = 1000;
   const Schedule schedule = Schedule::Sequence;
   const QList<QColor> colors = {"white", "blue", "green"};
   ON_CALL(SettingsProxyMock, GetStyle).WillByDefault(Return(style));
   ON_CALL(SettingsProxyMock, GetDuration).WillByDefault(Return(duration));
   ON_CALL(SettingsProxyMock, GetSchedule).WillByDefault(Return(schedule));
   ON_CALL(SettingsProxyMock, GetColors).WillByDefault(Return(colors));

   Worker->HandleSettingsChanged();
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(duration - 1);
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(2);
   EXPECT_EQ(2, spy.count());
}
