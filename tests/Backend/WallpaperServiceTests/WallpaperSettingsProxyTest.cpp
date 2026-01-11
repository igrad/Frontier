#include <WallpaperSettingsProxy.h>

#include <SettingsService/SettingsServiceMock.h>
#include <SettingsService/SettingsServicePointerHelper.h>

#include <TestMacros.h>

#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Settings;
using namespace testing;
using namespace Wallpaper;

class WallpaperSettingsProxyTest: public Test
{
public:
   WallpaperSettingsProxyTest()
      : SettingsSvcMock()
      , SettingsSvcHelper(&SettingsSvcMock)
      , Proxy()
      , Spy(&Proxy, &WallpaperSettingsProxy::SettingsChanged)
   {

   }

   NiceMock<SettingsServiceMock> SettingsSvcMock;
   SettingsServicePointerHelper SettingsSvcHelper;
   WallpaperSettingsProxy Proxy;
   QSignalSpy Spy;
};

TEST_F(WallpaperSettingsProxyTest, HandleSettingWallpaperColorsChanged1)
{
   const QStringList colorStrings = {"white", "blue", "green"};
   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperColors,
                                      QVariant(colorStrings));

   ASSERT_TRUE(Spy.wait());

   QList<QColor> colors;
   colors.push_back(QColor("white"));
   colors.push_back(QColor("blue"));
   colors.push_back(QColor("green"));
   EXPECT_EQ(colors, Proxy.GetColors());
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperDurationChanged1)
{
   const int duration = 3;

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperDuration,
                                      QVariant(duration));

   ASSERT_TRUE(Spy.wait());
   EXPECT_EQ(duration, Proxy.GetDuration());
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperFitsChanged1)
{
   QList<Fit> fits;
   fits.push_back(Fit::Center);
   fits.push_back(Fit::Fill);
   fits.push_back(Fit::Fit);

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperFits,
                                     QVariant::fromValue<QList<Fit>>(fits));

   ASSERT_TRUE(Spy.wait());

   EXPECT_EQ(fits, Proxy.GetFits());
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperImagePathsChanged1)
{
   const QStringList strings = {"1", "2", "3"};

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperImagePaths,
                                      QVariant(strings));

   ASSERT_TRUE(Spy.wait());

   EXPECT_EQ(strings, Proxy.GetPaths());
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperScheduleChanged1)
{
   const Schedule sched = Schedule::Sequence;

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperSchedule,
                                      QVariant::fromValue<Schedule>(sched));

   ASSERT_TRUE(Spy.wait());

   EXPECT_EQ(sched, Proxy.GetSchedule());
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperStyleChanged1)
{
   const Style style = Style::DynamicColor;

   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperStyle,
                                      QVariant::fromValue<Style>(style));

   ASSERT_TRUE(Spy.wait());

   EXPECT_EQ(style, Proxy.GetStyle());
}
