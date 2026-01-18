#include "TaskBarServiceMock.h"

using namespace TaskBar;

void TaskBarServiceMock::EmitViewDataChanged(const DisplayID& displayID, const ViewData& data)
{
   emit ViewDataChanged(displayID, data);
}
