#include <WallpaperService.h>

#include <SettingsService/SettingsServiceMock.h>
#include <SettingsService/SettingsServicePointerHelper.h>

#include <TestMacros.h>

#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Settings;
using namespace testing;
using namespace Wallpaper;

class WallpaperServiceTest: public Test
{
public:
   WallpaperServiceTest()
      : SettingsSvcMock()
      , SettingsSvcHelper(&SettingsSvcMock)
      , Service()
   {
   }

   NiceMock<SettingsServiceMock> SettingsSvcMock;
   SettingsServicePointerHelper SettingsSvcHelper;
   WallpaperService Service;
};

TEST_F(WallpaperServiceTest, HandleRotationTimeout1)
{
   QSignalSpy spy(&Service, &WallpaperService::WallpaperDataChanged);

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperStyle,
                                      QVariant::fromValue(Style::StaticColor));
   const int duration = 1000;
   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperDuration,
                                      1000);
   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperSchedule,
                                      QVariant::fromValue(Schedule::Sequence));

   ASSERT_TRUE(spy.wait());
   spy.clear();

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperColors,
                                      QStringList{"white", "blue", "green"});

   ASSERT_TRUE(spy.wait());
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(duration - 1);
   EXPECT_EQ(1, spy.count());

   Timer::AdvanceTime(2);
   EXPECT_EQ(2, spy.count());
}
