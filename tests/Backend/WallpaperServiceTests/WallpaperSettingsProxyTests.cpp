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
   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperColors,
                                             0,
                                             QVariant(colorStrings));

   QCoreApplication::processEvents();

   QList<QColor> colors;
   colors.push_back(QColor("white"));
   colors.push_back(QColor("blue"));
   colors.push_back(QColor("green"));
   EXPECT_EQ(colors, Proxy.GetColors(0));
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperDurationChanged1)
{
   const int duration = 3;

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperDuration,
                                             0,
                                             QVariant(duration));

   QCoreApplication::processEvents();
   EXPECT_EQ(duration, Proxy.GetDuration(0));
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperFitsChanged1)
{
   QList<Fit> fits;
   fits.push_back(Fit::Center);
   fits.push_back(Fit::Fill);
   fits.push_back(Fit::Fit);

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperFits,
                                             0,
                                             QVariant::fromValue<QList<Fit>>(fits));

   QCoreApplication::processEvents();

   EXPECT_EQ(fits, Proxy.GetFits(0));
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperImagePathsChanged1)
{
   const QStringList strings = {"1", "2", "3"};

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperImagePaths,
                                             0,
                                             QVariant(strings));

   QCoreApplication::processEvents();

   EXPECT_EQ(strings, Proxy.GetPaths(0));
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperScheduleChanged1)
{
   const Schedule sched = Schedule::Sequence;

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperSchedule,
                                             0,
                                             QVariant::fromValue<Schedule>(sched));

   QCoreApplication::processEvents();

   EXPECT_EQ(sched, Proxy.GetSchedule(0));
}

TEST_F(WallpaperSettingsProxyTest, HandleSettingsWallpaperStyleChanged1)
{
   const Style style = Style::DynamicColor;

   SettingsSvcMock.EmitDisplaySettingUpdated(Setting::WallpaperStyle,
                                             0,
                                             QVariant::fromValue<Style>(style));

   QCoreApplication::processEvents();

   EXPECT_EQ(style, Proxy.GetStyle(0));
}
