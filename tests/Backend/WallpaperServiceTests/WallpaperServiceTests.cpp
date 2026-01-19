#include <WallpaperService.h>

#include <SettingsService/SettingsServiceMock.h>
#include <SettingsService/SettingsServicePointerHelper.h>

#include <TestMacros.h>

#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Settings;
using namespace testing;
using namespace Wallpaper;

namespace
{
   constexpr const char* const DISPLAY_ID = "DISPLAY_ID";
}

class WallpaperServiceTest: public Test
{
public:
   WallpaperServiceTest()
      : SettingsSvcMock()
      , SettingsSvcHelper(&SettingsSvcMock)
      , Service()
   {
      SetUpSingleDisplay();
   }

   void SetUpSingleDisplay()
   {
      DisplayConfigEvent event;
      DisplayInfo info;
      info.ID = DISPLAY_ID;
      info.Number = 1;
      event.Displays[DISPLAY_ID] = {DisplayConfigEventType::Added, info};
      Service.HandleDisplayConfigChanged(event);
   }

   NiceMock<SettingsServiceMock> SettingsSvcMock;
   SettingsServicePointerHelper SettingsSvcHelper;
   WallpaperService Service;
};

TEST_F(WallpaperServiceTest, DISABLED_HandleRotationTimeout1)
{
   QSignalSpy spy(&Service, &WallpaperService::WallpaperDataChanged);

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperStyle,
                                             DISPLAY_ID,
                                             QVariant::fromValue(Style::StaticColor));
   const int duration = 1000;
   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperDuration,
                                             DISPLAY_ID,
                                             duration);
   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperSchedule,
                                             DISPLAY_ID,
                                             QVariant::fromValue(Schedule::Sequence));

   ASSERT_TRUE(spy.wait());
   spy.clear();

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperColors,
                                             DISPLAY_ID,
                                             QStringList{"white", "blue", "green"});

   ASSERT_TRUE(spy.wait());
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(duration - 1);
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(2);
   EXPECT_EQ(2, spy.count());
}
