#include "TaskBarServiceMock.h"

using namespace TaskBar;

void TaskBarServiceMock::EmitViewDataChanged(const ViewData& data)
{
   emit ViewDataChanged(data);
}
