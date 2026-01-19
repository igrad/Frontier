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
   GWT("Sequence schedule set up for color style",
       "The duration of has elapsed on the rotation timer",
       "The view data is updated");
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
   ASSERT_EQ(1, spy.count());

   ASSERT_EQ(2, spy[0].count());
   EXPECT_EQ(DISPLAY_ID, spy[0][0].value<DisplayID>());
   const ViewData data1 = spy[0][1].value<ViewData>();
   EXPECT_EQ(colors[0], data1.Color);
   EXPECT_EQ(style, data1.Style);

   Timer::AdvanceTime(duration - 1);
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(2);
   EXPECT_EQ(2, spy.count());
}

TEST_F(WallpaperServiceWorkerTest, HandleSettingsChanged1)
{
   GWT("Settings received",
       "HandleSettingsChanged called",
       "The view data is updated");
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
}

TEST_F(WallpaperServiceWorkerTest, DISABLED_HandleDisplayConfigChanged1)
{
}

TEST_F(WallpaperServiceWorkerTest, DISABLED_HandleDisplayRemoved1)
{
}
