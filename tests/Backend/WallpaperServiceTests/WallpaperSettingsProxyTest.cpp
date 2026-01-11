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
   SettingsSvcMock.EmitSettingUpdated(Setting::WallpaperColors,
                                      QVariant(QStringList({"white", "blue", "green"})));

   EXPECT_EQ(1, Spy.count());
}
