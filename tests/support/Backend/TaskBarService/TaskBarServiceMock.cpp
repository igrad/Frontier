#include "TaskBarServiceMock.h"

using namespace TaskBar;

void TaskBarServiceMock::EmitViewDataChanged(uint8_t display, const ViewData& data)
{
   emit ViewDataChanged(display, data);
}
