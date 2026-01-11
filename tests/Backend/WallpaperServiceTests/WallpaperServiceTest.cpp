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
      , Service(nullptr)
   {

   }

   void SetUpService()
   {
      Service.reset(new WallpaperService());
   }

   NiceMock<SettingsServiceMock> SettingsSvcMock;
   SettingsServicePointerHelper SettingsSvcHelper;
   std::unique_ptr<WallpaperService> Service;
};


TEST_F(WallpaperServiceTest, DISABLED_timerTest)
{
   SetUpService();

   QSignalSpy spy(Service.get(), &WallpaperService::WallpaperDataChanged);

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperDuration,
                                      1000);
   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperColors,
                                      QStringList{"white", "blue", "green"});
   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperSchedule,
                                      QVariant::fromValue(Schedule::Sequence));

   spy.clear();

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperStyle,
                                      QVariant::fromValue(Style::StaticColor));

   EXPECT_EQ(1, spy.count());
}
