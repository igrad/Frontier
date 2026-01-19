#include "TaskBarSettingsProxyMock.h"

using namespace TaskBar;

void TaskBarSettingsProxyMock::EmitSettingsChanged(const QString& displayID)
{
   emit SettingsChanged(displayID);
}
