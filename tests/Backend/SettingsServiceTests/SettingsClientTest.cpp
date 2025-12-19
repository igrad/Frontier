#include <SettingsClient.h>
#include <SettingsService/SettingsServiceMock.h>

#include <TestMacros.h>

using namespace Settings;
using namespace testing;

struct ServicePointerHelper
{
   ServicePointerHelper(NiceMock<SettingsServiceMock>* service)
   {
      SettingsClient::Service = service;
   }
};

class SettingsClientTest: public Test
{
public:
   SettingsClientTest()
      : Service()
      , PtrHelper(&Service)
      , Client("SettingsClientOwner")
   {
   }

   NiceMock<SettingsServiceMock> Service;
   ServicePointerHelper PtrHelper;
   SettingsClient Client;
};

TEST_F(SettingsClientTest, SubscribeToSetting1)
{
   GWT("Service and client are created AND subscribed to setting",
       "Setting is updated by service",
       "Client receives signal and invokes sub slot");
   Client.SubscribeToSetting(Setting::WallpaperSchedule, );
}
