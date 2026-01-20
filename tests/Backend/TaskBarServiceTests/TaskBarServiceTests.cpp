#include <TaskBarService.h>

#include <SettingsService/SettingsServiceMock.h>
#include <SettingsService/SettingsServicePointerHelper.h>

#include <TestMacros.h>

#include <QSignalSpy>
#include <gtest/gtest.h>

using namespace Settings;
using namespace TaskBar;
using namespace testing;

namespace
{
   constexpr const char* const DISPLAY_ID = "DISPLAY_ID";
}

class TaskBarServiceTest: public Test
{
public:
   TaskBarServiceTest()
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
   TaskBarService Service;
};
